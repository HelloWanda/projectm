/***************************************************************************
 *   Copyright (C) 2007 by carm   *
 *   carm@localhost   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifndef QPLAYLIST_FILEDIALOG_H
#define QPLAYLIST_FILEDIALOG_H

#include <QFileDialog>
#include <QStringList>
#include <QtDebug>

 class QPlaylistFileDialog : public QFileDialog
 {
     Q_OBJECT        // must include this if you use Qt signals/slots

 public:
     static QString OPEN_PLAYLIST_TITLE;
     static QString OPEN_PLAYLIST_OR_DIRECTORY_TITLE;
     static QString SAVE_PLAYLIST_TITLE;
     
     inline QPlaylistFileDialog(QWidget * parent = 0): 
		QFileDialog(parent, OPEN_PLAYLIST_OR_DIRECTORY_TITLE, QString()), m_directorySelectAllowed(true), m_fileSelectAllowed(true) {

	     updateFileMode(selectedFiles());
	     
	     //connect(this, SIGNAL(filesSelected(const QStringList&)),
		//     this, SLOT(updateFileMode(const QStringList&)));
	     
	     connect(this, SIGNAL(currentChanged(const QString&)),
		     this, SLOT(updateFileMode(const QString&)));
	}
	
	inline bool isPlaylistSaveMode() {
		return this->acceptMode() == QFileDialog::AcceptSave;
	}
	
	inline void setPlaylistSaveMode(bool isSaveMode) {
		if (isSaveMode) {
			this->setAcceptMode(QFileDialog::AcceptSave);	
			updateWindowTitle();
			updateFileMode(selectedFiles());
		}
		else {
			this->setAcceptMode(QFileDialog::AcceptOpen);
			updateWindowTitle();
			updateFileMode(selectedFiles());
		}
	}
	
	inline void setAllowDirectorySelect(bool isAllowed) {
		m_directorySelectAllowed = isAllowed;
		updateFileMode(selectedFiles());
		updateWindowTitle();
	}
		
	inline void setAllowFileSelect(bool isAllowed) {
		m_fileSelectAllowed = isAllowed;	
		updateFileMode(selectedFiles());
		updateWindowTitle();
	}
	
	
	inline bool isFileSelectAllowed() const {
		return m_fileSelectAllowed;	
	}
	
	
	inline bool isDirectorySelectAllowed() const {
		return m_directorySelectAllowed;		
	}
	
	
	
       ~QPlaylistFileDialog() { }

	 private:
		 
		 bool m_directorySelectAllowed;
		 bool m_fileSelectAllowed;
		 QString getFilter() {
			 QString filter;					 
			 if (isDirectorySelectAllowed()) {
			 
				 filter += "Directories";
			 }
		 
			 if (isFileSelectAllowed()) {			 
				 if (filter != QString())
					 filter += " and ";
		
				 filter += "Preset Playlists (*.ppl)";			 
			 } 
			 
			 return filter;		 
		 }
		 
		 void updateWindowTitle() {
			 if (isPlaylistSaveMode())
				 setWindowTitle(SAVE_PLAYLIST_TITLE);
			 else {
				 if (isDirectorySelectAllowed() && isFileSelectAllowed())
					 setWindowTitle(OPEN_PLAYLIST_OR_DIRECTORY_TITLE);
				 else
					 setWindowTitle(OPEN_PLAYLIST_TITLE);
			 }
		 }
		 
 private slots:
	 
	 void updateFileMode(const QString & fileName) {
		 
		 QString filter = getFilter();
		 		
		 if (QFileInfo(fileName).isDir()) {
			 if (isDirectorySelectAllowed())  
			 	this->setFileMode(QFileDialog::Directory);
			 else 
				 this->setFileMode(QFileDialog::ExistingFile);
		}
         	else if (QFileInfo(fileName).isFile()) {
			
			 if (isPlaylistSaveMode())
				 this->setFileMode(QFileDialog::AnyFile);
			 else if (isFileSelectAllowed()) 
				this->setFileMode(QFileDialog::ExistingFile);
			 else
				 this->setFileMode(QFileDialog::Directory);
		}
		
		else {
			if (isPlaylistSaveMode())
				this->setFileMode(QFileDialog::AnyFile);
			else
				this->setFileMode(QFileDialog::ExistingFile);
		}
				
		this->setFilter(filter);
	 }
	 
	 void updateFileMode(const QStringList & selectedFiles) {
		 if (selectedFiles.empty())
			 return;
		 updateFileMode(selectedFiles[0]);
	}
};


#endif
