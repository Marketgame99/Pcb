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

#ifndef LIBREPCB_GUI_LIBRARYGUI_H
#define LIBREPCB_GUI_LIBRARYGUI_H

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include <QtCore>

#include <memory>

/*******************************************************************************
 *  Namespace / Forward Declarations
 ******************************************************************************/
namespace librepcb {

class Library;

namespace gui {

class OpenedProject;

/*******************************************************************************
 *  Class LibraryGui
 ******************************************************************************/

/**
 * @brief GUI wrapper for a ::librepcb::Library
 */
class LibraryGui : public QObject {
  Q_OBJECT

public:
  // Constructors / Destructor
  LibraryGui() = delete;
  LibraryGui(OpenedProject& project, Library& library) noexcept;
  LibraryGui(const LibraryGui& other) noexcept = delete;
  virtual ~LibraryGui() noexcept;

  // Properties
  Q_PROPERTY(QString name READ getName NOTIFY nameChanged)

  // Getters
  const QString& getName() const noexcept;

signals:
  void nameChanged(const QString& name);

private:
  OpenedProject& mProject;
  QPointer<Library> mLibrary;
};

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace gui
}  // namespace librepcb

#endif
