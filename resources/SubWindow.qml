import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtQuick.Layouts 1.0


Item {
    id: subwindow

    /*!
        This property holds the sub-window's title text.

        There is no default title text.
    */
    property string title

    /*!
        This property holds the width of the content.
    */
    property real contentWidth: content.childrenRect.width

    /*!
        This property holds the height of the content.
    */
    property real contentHeight: content.childrenRect.height

    /*! \internal */
    property Component style: Qt.createComponent(Settings.theme() + "/SubWindowStyle.qml", subwindow)

    /*! \internal */
    default property alias data: content.data

    implicitWidth: Math.max(200, (loader.item ? loader.item.implicitWidth: 0) )
    implicitHeight: (loader.item ? loader.item.implicitHeight : 0)

    Layout.minimumWidth: implicitWidth
    Layout.minimumHeight: implicitHeight

    Accessible.role: Accessible.Grouping
    Accessible.name: title

    activeFocusOnTab: false

    Loader {
        id: loader
        anchors.fill: parent
        property int topMargin: (title.length > 0 ? 16 : 0) + content.margin
        property int bottomMargin: 4
        property int leftMargin: 4
        property int rightMargin: 4
        sourceComponent: styleLoader.item ? styleLoader.item.panel : null
        onLoaded: item.z = -1
        Loader {
            id: styleLoader
            property alias __control: subwindow
            sourceComponent: subwindow.style
        }
    }

    Item {
        id: content
        z: 1
        focus: true
        enabled: true

        property int margin: styleLoader.item ? styleLoader.item.margin : 0

        anchors.margins: margin
        //anchors.fill: styleLoader.item ? styleLoader.item.panel.content : parent
        //anchors.fill: loader
        anchors.topMargin: (loader.item && loader.item.titleBar ? loader.item.titleBar.height : 0) + margin
        anchors.leftMargin: margin
        anchors.rightMargin: margin
        anchors.bottomMargin: margin
        anchors.fill: parent
    }
}
