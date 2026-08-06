#include "EffectChainModel.h"

#include <Engine/DSP/EffectNode.h>

namespace Models {

EffectChainModel::EffectChainModel( Engine::AudioEngine& engine,
                                    Engine::NodeFactory& factory,
                                    QObject* parent ) : QAbstractListModel( parent ), _engine( engine ), _factory( factory ) {
}

int EffectChainModel::rowCount( const QModelIndex& parent ) const {
    if ( parent.isValid() )
        return 0;
    return _items.size();
}

QVariant EffectChainModel::data( const QModelIndex& index, int role ) const {
    if ( !index.isValid() || index.row() >= _items.size() )
        return {};
    const auto& item = _items[ index.row() ];
    switch ( role ) {
    case IdRole:
        return item.id;
    case TypeNameRole:
        return item.typeName;
    case EnabledRole:
        return item.enabled;
    }
    return {};
}

QHash<int, QByteArray> EffectChainModel::roleNames() const {
    return {
        { IdRole, "effectId" },
        { TypeNameRole, "typeName" },
        { EnabledRole, "enabled" },
    };
}

void EffectChainModel::addEffect( const QString& typeName ) {
    auto module = _factory.create( typeName.toStdString() );
    if ( !module )
        return;

    const QString id = nextId();
    auto node = std::make_shared<Engine::EffectNode>( std::move( module ) );

    beginInsertRows( {}, _items.size(), _items.size() );
    _items.append( { id, typeName } );
    endInsertRows();

    _engine.addNode( id.toStdString(), std::move( node ) );
    rebuildConnections();
    emit countChanged();
}

void EffectChainModel::removeEffect( int index ) {
    if ( index < 0 || index >= _items.size() )
        return;

    const QString id = _items[ index ].id;

    beginRemoveRows( {}, index, index );
    _items.removeAt( index );
    endRemoveRows();

    _engine.removeNode( id.toStdString() );
    rebuildConnections();
    emit countChanged();
}

void EffectChainModel::moveEffect( int from, int to ) {
    if ( from == to || from < 0 || to < 0 ||
         from >= _items.size() || to >= _items.size() )
        return;

    beginMoveRows( {}, from, from, {}, to > from ? to + 1 : to );
    _items.move( from, to );
    endMoveRows();

    rebuildConnections();
}

void EffectChainModel::rebuildConnections() {
    // Disconnect all existing edges then reconnect in current order
    for ( int i = 0; i + 1 < _items.size(); i++ ) {
        _engine.disconnectNodes( _items[ i ].id.toStdString(),
                                 _items[ i + 1 ].id.toStdString() );
    }
    for ( int i = 0; i + 1 < _items.size(); i++ ) {
        _engine.connectNodes( _items[ i ].id.toStdString(),
                              _items[ i + 1 ].id.toStdString() );
    }
}

QString EffectChainModel::nextId() {
    return QStringLiteral( "node-%1" ).arg( _idCounter++ );
}

} // namespace Models
