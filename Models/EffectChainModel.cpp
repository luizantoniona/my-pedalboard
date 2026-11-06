#include <Models/EffectChainModel.h>

#include <string>

#include <QVariantMap>

#include <Core/DSP/EffectNode.h>
#include <Core/Project/PedalRegistry.h>
#include <Engine/Audio/AudioEngine.h>

namespace Models {

EffectChainModel::EffectChainModel( Engine::AudioEngine& engine, Core::PedalRegistry& registry, QObject* parent ) :
    QAbstractListModel( parent ),
    _engine( engine ),
    _registry( registry ) {
}

EffectChainModel::~EffectChainModel() = default;

int EffectChainModel::rowCount( const QModelIndex& parent ) const {
    if ( parent.isValid() ) {
        return 0;
    }
    return static_cast<int>( _items.size() );
}

QVariant EffectChainModel::data( const QModelIndex& index, int role ) const {
    if ( !index.isValid() || index.row() < 0 || index.row() >= _items.size() ) {
        return QVariant();
    }

    const Item& item = _items.at( index.row() );
    switch ( role ) {
    case IdRole:
        return item.id;
    case PedalIdRole:
        return item.pedalId;
    case DisplayNameRole:
        return item.displayName;
    case TypeNameRole:
        return item.typeName;
    case EnabledRole:
        return item.node && item.node->effect() ? item.node->effect()->isEnabled() : false;
    case ParametersRole:
        return parameterList( item );
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> EffectChainModel::roleNames() const {
    return {
        { IdRole, "effectId" },
        { PedalIdRole, "pedalId" },
        { DisplayNameRole, "displayName" },
        { TypeNameRole, "typeName" },
        { EnabledRole, "effectEnabled" },
        { ParametersRole, "parameters" },
    };
}

QStringList EffectChainModel::availableTypes() const {
    QStringList ids;
    for ( const std::string& id : _registry.ids() ) {
        ids << QString::fromStdString( id );
    }
    return ids;
}

QStringList EffectChainModel::availableNames() const {
    QStringList names;
    for ( const std::string& id : _registry.ids() ) {
        names << QString::fromStdString( _registry.definition( id )->displayName );
    }
    return names;
}

void EffectChainModel::addEffect( const QString& pedalId ) {
    const Core::PedalDefinition* definition = _registry.definition( pedalId.toStdString() );
    std::unique_ptr<Core::DSPModule> effect = _registry.create( pedalId.toStdString() );
    if ( definition == nullptr || !effect ) {
        return;
    }

    Item item;
    item.typeName = QString::fromUtf8( effect->typeName().data(), static_cast<int>( effect->typeName().size() ) );
    item.node = std::make_shared<Core::EffectNode>( std::move( effect ) );
    item.id = nextId();
    item.pedalId = pedalId;
    item.displayName = QString::fromStdString( definition->displayName );

    beginInsertRows( QModelIndex(), static_cast<int>( _items.size() ), static_cast<int>( _items.size() ) );
    _items.append( item );
    endInsertRows();

    _engine.addNode( item.id.toStdString(), item.node );
    publishChain();
    emit countChanged();
}

void EffectChainModel::removeEffect( int row ) {
    if ( row < 0 || row >= _items.size() ) {
        return;
    }

    const QString id = _items.at( row ).id;

    beginRemoveRows( QModelIndex(), row, row );
    _items.removeAt( row );
    endRemoveRows();

    _engine.removeNode( id.toStdString() );
    publishChain();
    emit countChanged();
}

void EffectChainModel::moveEffect( int from, int to ) {
    if ( from < 0 || from >= _items.size() || to < 0 || to >= _items.size() || from == to ) {
        return;
    }

    const int destination = to > from ? to + 1 : to;
    beginMoveRows( QModelIndex(), from, from, QModelIndex(), destination );
    _items.move( from, to );
    endMoveRows();

    publishChain();
}

void EffectChainModel::clear() {
    if ( _items.isEmpty() ) {
        return;
    }

    beginResetModel();
    const QList<Item> removed = std::move( _items );
    _items.clear();
    endResetModel();

    for ( const Item& item : removed ) {
        _engine.removeNode( item.id.toStdString() );
    }
    publishChain();
    emit countChanged();
}

void EffectChainModel::setEffectEnabled( int row, bool enabled ) {
    if ( row < 0 || row >= _items.size() ) {
        return;
    }

    const Item& item = _items.at( row );
    Core::DSPModule* effect = item.node ? item.node->effect() : nullptr;
    if ( effect == nullptr || effect->isEnabled() == enabled ) {
        return;
    }

    effect->setEnabled( enabled );

    const QModelIndex changed = index( row, 0 );
    emit dataChanged( changed, changed, { EnabledRole } );
}

bool EffectChainModel::isEffectEnabled( int row ) const {
    if ( row < 0 || row >= _items.size() ) {
        return false;
    }
    const Item& item = _items.at( row );
    Core::DSPModule* effect = item.node ? item.node->effect() : nullptr;
    return effect != nullptr && effect->isEnabled();
}

void EffectChainModel::setParameter( int row, const QString& name, qreal value ) {
    if ( row < 0 || row >= _items.size() ) {
        return;
    }

    const Item& item = _items.at( row );
    Core::DSPModule* effect = item.node ? item.node->effect() : nullptr;
    if ( effect == nullptr ) {
        return;
    }
    const std::string parameterName = name.toStdString();
    effect->setParameter( parameterName, static_cast<float>( value ) );
}

qreal EffectChainModel::parameter( int row, const QString& name ) const {
    if ( row < 0 || row >= _items.size() ) {
        return 0.0;
    }

    const Item& item = _items.at( row );
    Core::DSPModule* effect = item.node ? item.node->effect() : nullptr;
    if ( effect == nullptr ) {
        return 0.0;
    }
    const std::string parameterName = name.toStdString();
    return effect->parameter( parameterName );
}

void EffectChainModel::publishChain() {
    std::vector<std::string> orderedIds;
    orderedIds.reserve( static_cast<std::size_t>( _items.size() ) );
    for ( const Item& item : _items ) {
        orderedIds.push_back( item.id.toStdString() );
    }
    _engine.setChain( std::move( orderedIds ) );
}

QVariantList EffectChainModel::parameterList( const Item& item ) const {
    QVariantList list;
    if ( !item.node || item.node->effect() == nullptr ) {
        return list;
    }

    for ( const Core::ParameterInfo& info : item.node->effect()->parameters() ) {
        QVariantMap entry;
        entry[ QStringLiteral( "name" ) ] = QString::fromUtf8( info.name.data(), static_cast<int>( info.name.size() ) );
        entry[ QStringLiteral( "label" ) ] = QString::fromUtf8( info.label.data(), static_cast<int>( info.label.size() ) );
        entry[ QStringLiteral( "unit" ) ] = QString::fromUtf8( info.unit.data(), static_cast<int>( info.unit.size() ) );
        entry[ QStringLiteral( "minimum" ) ] = info.minimum;
        entry[ QStringLiteral( "maximum" ) ] = info.maximum;
        entry[ QStringLiteral( "defaultValue" ) ] = info.defaultValue;
        list.append( entry );
    }
    return list;
}

QString EffectChainModel::nextId() {
    return QStringLiteral( "node-%1" ).arg( ++_idCounter );
}

} // namespace Models
