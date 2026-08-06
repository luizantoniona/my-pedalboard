#pragma once

#include <QAbstractListModel>
#include <QString>

#include <Engine/Audio/AudioEngine.h>
#include <Engine/Project/NodeFactory.h>

namespace Models {

// Maintains the ordered list of effects in the audio graph.
// Drives AudioEngine graph operations and exposes the chain to QML via ListView.
class EffectChainModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY( int count READ rowCount NOTIFY countChanged )

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        TypeNameRole,
        EnabledRole,
    };

    explicit EffectChainModel( Engine::AudioEngine& engine,
                               Engine::NodeFactory& factory,
                               QObject* parent = nullptr );

    int rowCount( const QModelIndex& parent = {} ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addEffect( const QString& typeName );
    Q_INVOKABLE void removeEffect( int index );
    Q_INVOKABLE void moveEffect( int from, int to );

signals:
    void countChanged();

private:
    struct Item {
        QString id;
        QString typeName;
        bool enabled{ true };
    };

    void rebuildConnections();
    QString nextId();

    Engine::AudioEngine& _engine;
    Engine::NodeFactory& _factory;
    QList<Item> _items;
    int _idCounter{ 0 };
};

} // namespace Models
