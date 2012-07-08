#ifndef _SKIN_BANK_H
#define _SKIN_BANK_H

#define QSAN_UI_LIBRARY_AVAILABLE

#include "card.h"
#include "qsanbutton.h"

#include <json/json.h>
#include <QString>
#include <QPixmap>
#include <QHash>
#include <QFont>
#include <QPen>
#include <QPainter>
#include <QGraphicsPixmapItem>

class QSanPixmapCache
{
public:
    // Load pixmap from a file and map it to the given key.
    static const QPixmap& getPixmap(const QString &key, const QString &fileName);
    // Load pixmap from a existing key.
    static const QPixmap& getPixmap(const QString &key);
    static bool contains(const QString &key); 
private:
    static QHash<QString, QPixmap> _m_pixmapBank;
};

class IQSanComponentSkin // interface class
{
public:
    class QSanSimpleTextFont {
    public:
        int* m_fontFace;
        QSize m_fontSize;
        int m_spacing;
        int m_weight;
        QColor m_color;
        bool m_vertical;
        QSanSimpleTextFont();
        bool tryParse(Json::Value arg);
        void paintText(QPainter* painter, QRect pos, Qt::Alignment align,
                       const QString &text) const;
        // this function's prototype is confusing. It will CLEAR ALL contents on the
        // QGraphicsPixmapItem passed in and then start drawing.
        void paintText(QGraphicsPixmapItem* item, QRect pos, 
                       Qt::Alignment align, const QString &text) const;
    protected:
        static QHash<QString, int*> _m_fontBank;
    };

    class QSanShadowTextFont : public QSanSimpleTextFont
    {
    public:
        int m_shadowRadius;
        double m_shadowDecadeFactor;
        QPoint m_shadowOffset;
        QColor m_shadowColor;
        bool tryParse(Json::Value arg);
        void paintText(QPainter* painter, QRect pos, Qt::Alignment align,
                       const QString &text) const;        
        // this function's prototype is confusing. It will CLEAR ALL contents on the
        // QGraphicsPixmapItem passed in and then start drawing.
        void paintText(QGraphicsPixmapItem* item, QRect pos, Qt::Alignment align,
                       const QString &text) const;
    };
    class AnchoredRect
    {
    public:
        QRect getTranslatedRect(QRect parentRect) const;
        QRect getTranslatedRect(QRect parentRect, QSize childSize) const;
        bool tryParse(Json::Value value);
    protected:
        Qt::Alignment m_anchorChild;
        Qt::Alignment m_anchorParent;
        QPoint m_offset;
        QSize m_fixedSize;
        bool m_useFixedSize;
    };
    static const char* S_SKIN_KEY_DEFAULT;
    bool load(const QString &layoutConfigFileName, const QString &imageConfigFileName,
              const QString &audioConfigFileName);
    QPixmap getPixmap(const QString &key, const QString &arg = QString()) const;
    QPixmap getPixmapFileName(const QString &key) const;
    QPixmap getPixmapFromFileName(const QString &fileName) const;
    QStringList getAudioFileNames(const QString &key) const;
    QStringList getAnimationFileNames() const;
    QString getRandomAudioFileName(const QString &key) const;
    bool isImageKeyDefined(const QString &key) const;
protected:
    virtual bool _loadLayoutConfig(const Json::Value &config) = 0;
    virtual bool _loadImageConfig(const Json::Value &config);
    QString _readConfig(const Json::Value &dictionary, const QString &key,
                        const QString &defaultValue = QString()) const;
    QString _readImageConfig(const QString &key, QRect &clipRegion, bool &clipping,
                             QSize &newScale, bool scaled,
                             const QString &defaultValue = QString()) const;
    
    Json::Value _m_imageConfig;
    Json::Value _m_audioConfig;
    // image key -> image file name
    static QHash<QString, QString> S_IMAGE_KEY2FILE;
    static QHash<QString, QPixmap> S_IMAGE_KEY2PIXMAP;
    // image group key -> image keys
    // for example,
    // "generalIcon-1-default" -> "generalIcon-1-zhangliao", "generalIcon-1-liubie", ...
    static QHash<QString, QList<QString>> S_IMAGE_GROUP_KEYS;

};

class QSanRoomSkin : public IQSanComponentSkin
{
public:
    struct RoomLayout {
        int m_scenePadding;
        int m_roleBoxHeight;
        int m_chatTextBoxHeight;
        int m_discardPileMinWidth;
        int m_discardPilePadding;
        double m_logBoxHeightPercentage;
        double m_chatBoxHeightPercentage;
        double m_infoPlaneWidthPercentage;
        double m_photoDashboardPadding;
        int m_photoHDistance;
        int m_photoVDistance;
        QSize m_minimumSceneSize;
        QSize m_maximumSceneSize;
    };

    struct PlayerCardContainerLayout
    {        
        int m_normalHeight;
        QRect m_boundingRect;
        QRect m_focusFrameArea;
        QRect m_handCardArea;
        
        // equips
        QRect m_equipAreas[4];
        QRect m_equipImageArea;
        QRect m_equipTextArea;
        QRect m_equipSuitArea;
        QRect m_equipDistanceArea;
        QRect m_equipPointArea;
        QSanShadowTextFont m_equipFont;
        QSanShadowTextFont m_equipPointFont;

        // delayed trick area
        QRect m_delayedTrickFirstRegion;
        QPoint m_delayedTrickStep;
        
        AnchoredRect m_markTextArea;
        QPoint m_roleComboBoxPos;

        // photo area
        QRect m_avatarArea;
        int m_avatarSize;
        QRect m_smallAvatarArea;
        int m_smallAvatarSize;
        QRect m_avatarNameArea;
        QRect m_smallAvatarNameArea;
        QSanShadowTextFont m_avatarNameFont;
        QSanShadowTextFont m_smallAvatarNameFont;
        QRect m_kingdomIconArea;
        QRect m_kingdomMaskArea;
        QSanShadowTextFont m_handCardFont;
        QRect m_screenNameArea;
        QSanShadowTextFont m_screenNameFont;
        
        // progress bar and other controls
        bool m_isProgressBarHorizontal;
        AnchoredRect m_progressBarArea;
        QSize m_magatamaSize;
        bool m_magatamasHorizontal;
        bool m_magatamasBgVisible;
        QPoint m_magatamasAnchor;
        Qt::Alignment m_magatamasAlign;

        AnchoredRect m_phaseArea;        
        
        // private pile (e.g. 7 stars, buqu)
        QPoint m_privatePileStartPos;
        QPoint m_privatePileStep;
        QSize m_privatePileButtonSize;

        // various icons
        QRect m_actionedIconRegion;
        QRect m_saveMeIconRegion;
        QRect m_chainedIconRegion;
        QRect m_readyIconRegion;        
        AnchoredRect m_deathIconRegion;
        QRect m_votesIconRegion;
        QColor m_drankMaskColor;
        QColor m_deathEffectColor;
    };

    struct PhotoLayout : public PlayerCardContainerLayout
    {
        int m_normalWidth;
        QRect m_mainFrameArea;
        QRect m_cardMoveRegion;        
        QRect m_onlineStatusArea;
        QSanShadowTextFont m_onlineStatusFont;
        QColor m_onlineStatusBgColor;
        QRect m_skillNameArea;
        QSanShadowTextFont m_skillNameFont;        
    };

    struct DashboardLayout : public PlayerCardContainerLayout
    {
        int m_leftWidth, m_rightWidth;
        int m_floatingAreaHeight;
        QSize m_buttonSetSize;
        QRect m_confirmButtonArea;
        QRect m_cancelButtonArea;
        QRect m_discardButtonArea;
        QRect m_trustButtonArea;
        QSize m_skillButtonsSize[3];
        QRect m_skillTextArea[3];
        QPoint m_equipBorderPos;
        QPoint m_equipSelectedOffset;
        int m_disperseWidth;
        QSanShadowTextFont m_skillTextFonts[3];
        QColor m_skillTextColors[QSanButton::S_NUM_BUTTON_STATES *
                                 QSanInvokeSkillButton::S_NUM_SKILL_TYPES];
        QColor m_skillTextShadowColors[QSanButton::S_NUM_BUTTON_STATES *
                                       QSanInvokeSkillButton::S_NUM_SKILL_TYPES];

        QSanShadowTextFont getSkillTextFont(QSanButton::ButtonState state,
                                            QSanInvokeSkillButton::SkillType type,
                                            QSanInvokeSkillButton::SkillButtonWidth width) const;        
    };

    struct CommonLayout
    {
        // card related
        int m_cardNormalWidth;
        int m_cardNormalHeight;
        QRect m_cardMainArea;
        QRect m_cardSuitArea;
        QRect m_cardNumberArea;
        QRect m_cardFootnoteArea;
        QRect m_cardAvatarArea;
        QRect m_cardFrameArea;
        QSanShadowTextFont m_cardFootnoteFont;
        QSanShadowTextFont m_hpFont[6];
        int m_hpExtraSpaceHolder;

        // dialogs        
        // when # of generals < switchIconSizeThreadshold
        QSize m_chooseGeneralBoxSparseIconSize;
        // when # of generals < switchIconSizeThreadshold
        QSize m_chooseGeneralBoxDenseIconSize;
        int m_chooseGeneralBoxSwitchIconSizeThreshold;
    };
    enum GeneralIconSize
    {
        S_GENERAL_ICON_SIZE_TINY,
        S_GENERAL_ICON_SIZE_SMALL,
        S_GENERAL_ICON_SIZE_LARGE,
        S_GENERAL_ICON_SIZE_CARD
    };

    const RoomLayout& getRoomLayout() const;
    const PhotoLayout& getPhotoLayout() const;
    const CommonLayout& getCommonLayout() const;
    const DashboardLayout& getDashboardLayout() const;
    
    // @todo: these two functions are currently only used to generate HTML when prompt whether to
    // use Nullification. Get rid of them in the future.
    QString getCardMainPixmapPath(const QString &cardName) const;
    QString getGeneralPixmapPath(const QString &generalName, GeneralIconSize size) const;

    QString getButtonPixmapPath(const QString &groupName, const QString &buttonName, QSanButton::ButtonState state) const;
    QPixmap getButtonPixmap(const QString &groupName, const QString &buttonName, QSanButton::ButtonState state) const;
    QPixmap getSkillButtonPixmap(QSanButton::ButtonState state,
                                 QSanInvokeSkillButton::SkillType type, 
                                 QSanInvokeSkillButton::SkillButtonWidth width) const;
    QPixmap getCardMainPixmap(const QString &cardName) const;
    QPixmap getCardSuitPixmap(Card::Suit suit) const;
    QPixmap getCardNumberPixmap(int point, bool isBlack) const;
    QPixmap getCardJudgeIconPixmap(const QString &judgeName) const;
    QPixmap getCardFramePixmap(const QString &frameType) const;
    QPixmap getCardAvatarPixmap(const QString &generalName) const;
    QPixmap getGeneralPixmap(const QString &generalName, GeneralIconSize size) const;
    QString getPlayerAudioEffectPath(const QString &eventName, bool isMale, int index = -1) const;
    QString getPlayerAudioEffectPath(const QString &eventName, const QString &category, int index = -1) const;
    QPixmap getProgressBarPixmap(int percentile) const;

    // static consts
    // main keys
    static const char* S_SKIN_KEY_DASHBOARD;
    static const char* S_SKIN_KEY_PHOTO;
    static const char* S_SKIN_KEY_COMMON;
    static const char* S_SKIN_KEY_ROOM;    

    // button
    static const char* S_SKIN_KEY_BUTTON;
    static const char* S_SKIN_KEY_DASHBOARD_BUTTON_SET_BG;
    static const char* S_SKIN_KEY_BUTTON_DASHBOARD_CONFIRM;
    static const char* S_SKIN_KEY_BUTTON_DASHBOARD_CANCEL;
    static const char* S_SKIN_KEY_BUTTON_DASHBOARD_DISCARD;
    static const char* S_SKIN_KEY_BUTTON_DASHBOARD_TRUST;
    static const char* S_SKIN_KEY_PLATTER;
    static const char* S_SKIN_KEY_BUTTON_SKILL;

    // player container
    static const char* S_SKIN_KEY_MAINFRAME;
    static const char* S_SKIN_KEY_LEFTFRAME;
    static const char* S_SKIN_KEY_RIGHTFRAME;
    static const char* S_SKIN_KEY_MIDDLEFRAME;
    static const char* S_SKIN_KEY_HANDCARDNUM;
    static const char* S_SKIN_KEY_FACETURNEDMASK;
    static const char* S_SKIN_KEY_BLANK_GENERAL;
    static const char* S_SKIN_KEY_CHAIN;
    static const char* S_SKIN_KEY_PHASE;
    static const char* S_SKIN_KEY_SELECTED_FRAME;
    static const char* S_SKIN_KEY_FOCUS_FRAME;
    static const char* S_SKIN_KEY_SAVE_ME_ICON;
    static const char* S_SKIN_KEY_ACTIONED_ICON;
    static const char* S_SKIN_KEY_KINGDOM_ICON;
    static const char* S_SKIN_KEY_KINGDOM_COLOR_MASK;
    static const char* S_SKIN_KEY_VOTES_NUMBER;
    static const char* S_SKIN_KEY_HAND_CARD_BACK;
    static const char* S_SKIN_KEY_HAND_CARD_SUIT;
    static const char* S_SKIN_KEY_JUDGE_CARD_ICON;
    static const char* S_SKIN_KEY_HAND_CARD_MAIN_PHOTO;
    static const char* S_SKIN_KEY_HAND_CARD_NUMBER_BLACK;
    static const char* S_SKIN_KEY_HAND_CARD_NUMBER_RED;
    static const char* S_SKIN_KEY_HAND_CARD_FRAME;
    static const char* S_SKIN_KEY_PLAYER_GENERAL_ICON;
    static const char* S_SKIN_KEY_MAGATAMAS_BG;
    static const char* S_SKIN_KEY_MAGATAMAS;
    static const char* S_SKIN_KEY_PLAYER_AUDIO_EFFECT;
    static const char* S_SKIN_KEY_SYSTEM_AUDIO_EFFECT;
    static const char* S_SKIN_KEY_EQUIP_ICON;
    static const char* S_SKIN_KEY_PROGRESS_BAR_IMAGE;

    // Animations
    static const char* S_SKIN_KEY_ANIMATIONS;

    // The following ones are to be moved to lobby skin later
    static const char* S_SKIN_KEY_READY_ICON;

protected:
    RoomLayout _m_roomLayout;
    PhotoLayout _m_photoLayout;
    CommonLayout _m_commonLayout;
    DashboardLayout _m_dashboardLayout;
    virtual bool _loadLayoutConfig(const Json::Value &layoutConfig);
};

class QSanSkinScheme
{
// Why do we need another layer above room skin? Because we may add lobby, login interface
// in the future; and we may need to assemble a set of different skins into a scheme.
public:
    bool load(Json::Value configs);
    const QSanRoomSkin& getRoomSkin() const;
protected:
    QSanRoomSkin _m_roomSkin;
};

class QSanSkinFactory
{
public:
    static QSanSkinFactory& getInstance();
    const QString& getCurrentSkinName() const;
    const QSanSkinScheme& getCurrentSkinScheme();
    bool switchSkin(QString skinName);
protected:
    QSanSkinFactory(const char* fileName);
    static QSanSkinFactory* _sm_singleton;
    QSanSkinScheme _sm_currentSkin;
    Json::Value _m_skinList;
    QString _m_skinName;
};

#define G_ROOM_SKIN (QSanSkinFactory::getInstance().getCurrentSkinScheme().getRoomSkin())
#define G_DASHBOARD_LAYOUT (G_ROOM_SKIN.getDashboardLayout())
#define G_ROOM_LAYOUT (G_ROOM_SKIN.getRoomLayout())
#define G_PHOTO_LAYOUT (G_ROOM_SKIN.getPhotoLayout())
#define G_COMMON_LAYOUT (G_ROOM_SKIN.getCommonLayout())

#endif
