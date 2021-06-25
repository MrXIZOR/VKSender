#ifndef FETCHER_H
#define FETCHER_H

#include <QObject>
#include <QString>
#include <QNetworkReply>
#include <QPixmap>
#include <QJsonParseError>

#include "types.h"
#include "repository.h"

struct Request
{
    struct GroupById
    {
        QString method = "groups.getById";
        QString groupIds = "";
        QString fields = "can_post";
    };

    struct WallPost
    {
        QString method = "wall.post";
        QString groupIds = "";
        QString description = "";
        QString message = "";
        QString groupId = "";
        QString attachments = "";
    };
    struct Photos
    {
        struct getWallUploadServer
        {
            QString method = "photos.getWallUploadServer";
            QString groupId = "";
        };

        struct saveWallPhoto
        {
            QString method = "photos.saveWallPhoto";
            QString hash = "";
            QString photo = "";
            int server = 0;
            QString groupId = "";
        };

        getWallUploadServer getWallUploadServer;
        saveWallPhoto saveWallPhoto;
    };

    struct UsersGet
    {
        QString method = "users.get";
        QString user_ids = "";
        QString fields = "";
        QString name_case = "";
    };

    struct GroupsGet
    {
        QString method = "groups.get";
        QString extended = "1";
        QString filter = "";
        QString fields = "can_post";
        QString count = "1000";
    };

    struct WallDelete
    {
        QString method = "wall.delete";
        QString ownerId = "";
        QString postId = "";
    };

    GroupById groupById;
    WallPost wallPost;
    Photos photos;
    WallDelete wallDelete;
    UsersGet usersGet;
    GroupsGet groupsGet;

    QString implicitFlowAccessToken = "";
    QString userId = "";
    QString apiVersion = "5.131";
};

struct UserInfo
{
    int userId = 0;
    QString userName = "";
    Link userPhoto100Link = "";
    QPixmap userPhoto100;
    QVector<Group> userGroups;
};

class Fetcher : public QObject
{
    Q_OBJECT

signals:
    void updatedGroupData(QVector<Group> groups);
    void updatedPhoto(QUuid id);
    void sentMessage(QUuid id, Group group, PostNumber number);
    void deletedPost(QString postId, QString ownerId);
    void userPhoto100Update();
    void userNameUpdate();
    void userGroupsUpdate();
    void onGroupUpdated(Group group);
    void sendingFinished(QUuid id);

public:
    explicit Fetcher(QObject *parent = nullptr);
    ~Fetcher();

    void setRepository(const std::shared_ptr<Repository> repository);
    bool tokenIsEmpty() const;
    void setAccessToken(QString token);
    void getGroupInfoById(const QString &id);

    const QPixmap &getUserPhoto100() const;
    const QString &getUserName() const;
    const QVector<Group> &getUserGroups() const;

    void sendMessage(const MessagePack &pack);

public slots:
    void onPostDelete(const QString postId, const QString ownerId);
    void onUserDataUpdate();

private:
    bool isReplyErrorReturned(const QNetworkReply &reply) const;
    bool isJsonErrorReturned(const QJsonParseError &error) const;
    bool isServerErrorReturned(const QJsonDocument &document) const;

    void setUserPhoto100(const QPixmap &userphoto);
    void setUserName(const QString &userName);
    void setUserId(const int &id);
    void setUserPhoto100Link(const QString &link);
    void setUserGroups(const QVector<Group> groups);

    void downloadUserInfo();
    void downloadUserGroups();
    QPixmap *uploadPhoto(const QUrl &url) const;

private:
    const Link mVkApiLink = "https://api.vk.com/method/";
    QNetworkAccessManager *mNetworkManager;

    UserInfo mUserInfo;

    std::shared_ptr<Repository> mRepository = nullptr;
};

#endif // FETCHER_H
