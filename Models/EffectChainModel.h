#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QString>

namespace Engine {
class AudioEngine;
}

namespace Core {
class NodeRegistry;
}

namespace Models {

// Ordered list of effects in the chain. Drives the graph and exposes the chain
// to QML; the graph edges are rebuilt from scratch on every structural change.
class EffectChainModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY( int count READ rowCount NOTIFY countChanged )
    Q_PROPERTY( QStringList availableTypes READ availableTypes CONSTANT )

public:
    enum RoleEnum {
        IdRole = Qt::UserRole + 1,
        TypeNameRole,
        EnabledRole,
    };

    explicit EffectChainModel( Engine::AudioEngine& engine, Core::NodeRegistry& registry, QObject* parent = nullptr );

    int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;

    QStringList availableTypes() const;

    Q_INVOKABLE void addEffect( const QString& typeName );
    Q_INVOKABLE void removeEffect( int row );
    Q_INVOKABLE void moveEffect( int from, int to );
    Q_INVOKABLE void setEffectEnabled( int row, bool enabled );

signals:
    void countChanged();

private:
    class Item {

    public:
        QString id;
        QString typeName;
        bool enabled{ true };
    };

    void rebuildConnections();
    QString nextId();

    Engine::AudioEngine& _engine;
    Core::NodeRegistry& _registry;
    QList<Item> _items;
    int _idCounter{ 0 };
};

} // namespace Models
