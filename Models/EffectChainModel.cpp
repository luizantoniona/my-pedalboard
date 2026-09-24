#include <Models/EffectChainModel.h>

#include <Core/DSP/EffectNode.h>
#include <Core/Project/NodeRegistry.h>
#include <Engine/Audio/AudioEngine.h>

namespace Models {

EffectChainModel::EffectChainModel( Engine::AudioEngine& engine, Core::NodeRegistry& registry, QObject* parent ) :
    QAbstractListModel( parent ),
    _engine( engine ),
    _registry( registry ) {
}

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
    case TypeNameRole:
        return item.typeName;
    case EnabledRole:
        return item.enabled;
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> EffectChainModel::roleNames() const {
    return {
        { IdRole, "effectId" },
        { TypeNameRole, "typeName" },
        { EnabledRole, "enabled" },
    };
}

QStringList EffectChainModel::availableTypes() const {
    QStringList names;
    for ( const std::string& name : _registry.typeNames() ) {
        names << QString::fromStdString( name );
    }
    return names;
}

void EffectChainModel::addEffect( const QString& typeName ) {
    std::unique_ptr<Core::DSPModule> effect = _registry.create( typeName.toStdString() );
    if ( !effect ) {
        return;
    }

    Item item;
    item.id = nextId();
    item.typeName = typeName;

    beginInsertRows( QModelIndex(), static_cast<int>( _items.size() ), static_cast<int>( _items.size() ) );
    _items.append( item );
    endInsertRows();

    _engine.addNode( item.id.toStdString(), std::make_shared<Core::EffectNode>( std::move( effect ) ) );
    rebuildConnections();
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
    rebuildConnections();
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

    rebuildConnections();
}

void EffectChainModel::setEffectEnabled( int row, bool enabled ) {
    if ( row < 0 || row >= _items.size() || _items.at( row ).enabled == enabled ) {
        return;
    }

    _items[ row ].enabled = enabled;
    const QModelIndex changed = index( row, 0 );
    emit dataChanged( changed, changed, { EnabledRole } );
}

// Every structural change republishes the whole edge list: the chain is short
// and the graph rebuild happens off the audio thread.
void EffectChainModel::rebuildConnections() {
    for ( int i = 0; i + 1 < _items.size(); i++ ) {
        _engine.connectNodes( _items.at( i ).id.toStdString(), _items.at( i + 1 ).id.toStdString() );
    }
}

QString EffectChainModel::nextId() {
    return QStringLiteral( "node-%1" ).arg( ++_idCounter );
}

} // namespace Models
