/***************************************************************************
**
** This file is part of the qmlogre example on http://qt.gitorious.org.
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).*
**
** All rights reserved.
** Contact:  Nokia Corporation (qt-info@nokia.com)
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**
**  * Redistributions of source code must retain the above copyright notice,
**    this list of conditions and the following disclaimer.
**  * Redistributions in binary form must reproduce the above copyright notice,
**    this list of conditions and the following disclaimer in the documentation
**    and/or other materials provided with ** the distribution.
**  * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor the
**    names of its contributors may be used to endorse or promote products
**    derived from this software without specific ** prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
** DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
** SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
** CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
** OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
**************************************************************************/

#ifndef OGREITEM_H
#define OGREITEM_H

#include <QtQuick/QQuickItem>
#include <QtQuick/QSGGeometry>
#include <QtQuick/QSGTextureMaterial>
#include <QtCore/QPropertyAnimation>

#include <OGRE/OgreTexture.h>

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
#define OGRE_PLUGIN_DIR STRINGIFY(OGRE_PLUGIN_VAR)

class QSGSimpleTextureNode;

namespace Ogre {
class Root;
class Camera;
class SceneManager;
class RenderTexture;
class Viewport;
class RenderTarget;
}

class CameraNodeObject;

#ifndef GLuint
    typedef unsigned int GLuint;
#endif

class OgreItem : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QObject *camera READ camera)

public:
    OgreItem(QQuickItem *parent = 0);
    ~OgreItem();

    QObject *camera() const { return m_cameraQObject; }

    void setSize(const QSize &size);
    QSize size() const { return m_size; }

    void setAAEnabled(bool enable);

    void updateFBO();

    void init();

    GLuint getOgreFBO();

    void saveOgreState();
    void restoreOgreState();

protected:
    virtual QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

    void timerEvent(QTimerEvent *);

private:
    int m_timerID;

    QObject *m_cameraQObject;

    QSGSimpleTextureNode *m_node;

    QSGTextureMaterial m_material;
    QSGOpaqueTextureMaterial m_materialO;
    QSGGeometry m_geometry;
    QSGTexture *m_texture;

    int m_samples;
    bool m_AAEnabled;
    QSize m_size;

    Ogre::Root *m_root;
    Ogre::Camera *m_camera;
    Ogre::SceneManager *m_sceneManager;
    Ogre::RenderTexture *m_renderTexture;
    Ogre::Viewport *m_viewport;
    Ogre::TexturePtr rtt_texture;
    Ogre::RenderWindow *m_window;

    GLuint m_ogreFBO;

    CameraNodeObject *m_cameraObject;

    bool m_initialized;
    bool m_dirtyFBO;
};

#endif // OGREITEM_H
