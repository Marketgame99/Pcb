/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * https://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include "editorwindow.h"

#include "editorapplication.h"
#include "objectlistmodel.h"
#include "openedproject.h"
#include "schematicgui.h"
#include <librepcb/core/application.h>
#include <librepcb/core/fileio/filepath.h>

#include <QtCore>

/*******************************************************************************
 *  Namespace
 ******************************************************************************/
namespace librepcb {
namespace gui {

/*******************************************************************************
 *  Constructors / Destructor
 ******************************************************************************/

EditorWindow::EditorWindow(EditorApplication& application)
  : QObject(&application),
    mApplication(application),
    mTitle(QString("LibrePCB %1").arg(qApp->applicationVersion())),
    mEngine(new QQmlApplicationEngine(this)) {
  const QUrl url =
      Application::getResourcesDir().getPathTo("qml/MainWindow.qml").toQUrl();
  connect(
      mEngine.data(), &QQmlApplicationEngine::objectCreated, this,
      [url](QObject* obj, const QUrl& objUrl) {
        if ((!obj) && (objUrl == url)) {
          qFatal("Failed to load the QML window!");  // Quits the application!
        }
      },
      Qt::QueuedConnection);
  mEngine->rootContext()->setContextProperty("cppApp", &mApplication);
  mEngine->rootContext()->setContextProperty("cppWindow", this);
  mEngine->load(url);

  // For testing, open a project automatically.
  QTimer::singleShot(200, this, [this]() {
    QSettings s;
    const FilePath fp(
        s.value("app/last_open_project", mApplication.getWorkspacePath())
            .toString());
    if (fp.isValid()) {
      setCurrentProject(mApplication.openProject(fp));
    }
  });
}

EditorWindow::~EditorWindow() noexcept {
}

/*******************************************************************************
 *  Getters
 ******************************************************************************/

QObject* EditorWindow::getCurrentProject() noexcept {
  return mCurrentProject.get();
}

QObject* EditorWindow::getCurrentSchematic() noexcept {
  return mCurrentSchematic.get();
}

/*******************************************************************************
 *  GUI Handlers
 ******************************************************************************/

bool EditorWindow::createProject() noexcept {
  if (auto p = mApplication.createProject()) {
    setCurrentProject(p);
    return true;
  }
  return false;
}

bool EditorWindow::openProject() noexcept {
  if (auto p = mApplication.openProject()) {
    setCurrentProject(p);
    return true;
  }
  return false;
}

/*******************************************************************************
 *  Private Methods
 ******************************************************************************/

void EditorWindow::setCurrentProject(
    std::shared_ptr<OpenedProject> p) noexcept {
  if (p != mCurrentProject) {
    setCurrentSchematic(nullptr);
    mCurrentProject = p;
    emit currentProjectChanged();
  }
}

void EditorWindow::setCurrentSchematic(std::shared_ptr<SchematicGui> s) noexcept {
  if (s != mCurrentSchematic) {
    mCurrentSchematic = s;
    emit currentSchematicChanged();
  }
}

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace gui
}  // namespace librepcb
