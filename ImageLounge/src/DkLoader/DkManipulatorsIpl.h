/*******************************************************************************************************
 nomacs is a fast and small image viewer with the capability of synchronizing multiple instances
 
 Copyright (C) 2011-2016 Markus Diem <markus@nomacs.org>
 Copyright (C) 2011-2016 Stefan Fiel <stefan@nomacs.org>
 Copyright (C) 2011-2016 Florian Kleber <florian@nomacs.org>

 This file is part of nomacs.

 nomacs is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 nomacs is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 related links:
 [1] http://www.nomacs.org/
 [2] https://github.com/nomacs/
 [3] http://download.nomacs.org
 *******************************************************************************************************/

#pragma once

#include "DkManipulators.h"

#pragma warning(push, 0)	// no warnings from includes
#include <QAction>
#pragma warning(pop)

#pragma warning(disable: 4251)	// TODO: remove

#ifndef DllLoaderExport
#ifdef DK_LOADER_DLL_EXPORT
#define DllLoaderExport Q_DECL_EXPORT
#elif DK_DLL_IMPORT
#define DllLoaderExport Q_DECL_IMPORT
#else
#define DllLoaderExport Q_DECL_IMPORT
#endif
#endif

// Qt defines

namespace nmc {

// nomacs defines

class DkGrayScaleManipulator : public DkBaseManipulator {

public:
	DkGrayScaleManipulator(QAction* action = 0);

	QImage apply(const QImage& img) const override;
	QString errorMessage() const override;
};

class DkAutoAdjustManipulator : public DkBaseManipulator {

public:
	DkAutoAdjustManipulator(QAction* action = 0);

	QImage apply(const QImage& img) const override;
	QString errorMessage() const override;
};

class DkNormalizeManipulator : public DkBaseManipulator {

public:
	DkNormalizeManipulator(QAction* action = 0);

	QImage apply(const QImage& img) const override;
	QString errorMessage() const override;
};
}