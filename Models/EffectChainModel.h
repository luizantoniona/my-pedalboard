#pragma once

#include <memory>

#include <QAbstractListModel>
#include <QList>
#include <QString>
#include <QVariantList>

namespace Engine {
class AudioEngine;
}

namespace Core {
class EffectNode;
class PedalRegistry;
} // namespace Core

namespace Models {

class EffectChainModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY( int count READ rowCount NOTIFY countChanged )
    Q_PROPERTY( QStringList availableTypes READ availableTypes CONSTANT )
    Q_PROPERTY( QStringList availableNames READ availableNames CONSTANT )

public:
    enum RoleEnum {
        IdRole = Qt::UserRole + 1,
        PedalIdRole,
        DisplayNameRole,
        TypeNameRole,
        EnabledRole,
        ParametersRole,
    };

    explicit EffectChainModel( Engine::AudioEngine& engine, Core::PedalRegistry& registry, QObject* parent = nullptr );
    ~EffectChainModel() override;

    int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
    QVariant data( const QModelIndex& index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;

    QStringList availableTypes() const;
    QStringList availableNames() const;

    Q_INVOKABLE void addEffect( const QString& pedalId );
    Q_INVOKABLE void removeEffect( int row );
    Q_INVOKABLE void moveEffect( int from, int to );
    Q_INVOKABLE void clear();

    Q_INVOKABLE void setEffectEnabled( int row, bool enabled );
    Q_INVOKABLE bool isEffectEnabled( int row ) const;

    Q_INVOKABLE void setParameter( int row, const QString& name, qreal value );
    Q_INVOKABLE qreal parameter( int row, const QString& name ) const;

signals:
    void countChanged();

private:
    class Item {

    public:
        std::shared_ptr<Core::EffectNode> node;
        QString id;
        QString pedalId;
        QString displayName;
        QString typeName;
    };

    void publishChain();
    QVariantList parameterList( const Item& item ) const;
    QString nextId();

    Engine::AudioEngine& _engine;
    Core::PedalRegistry& _registry;
    QList<Item> _items;
    int _idCounter{ 0 };
};

} // namespace Models
