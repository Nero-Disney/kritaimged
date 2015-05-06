/*
 * Copyright (C) 2009 Boudewijn Rempt <boud@valdyas.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#include "kis_psd_test.h"


#include <QTest>
#include <QCoreApplication>

#include <qtest_kde.h>

#include "filestest.h"

#ifndef FILES_DATA_DIR
#error "FILES_DATA_DIR not set. A directory with the data used for testing the importing of files in krita"
#endif


void KisPSDTest::testFiles()
{
    TestUtil::testFiles(QString(FILES_DATA_DIR) + "/sources", QStringList());
}

void KisPSDTest::testOpening()
{
    QFileInfo sourceFileInfo(QString(FILES_DATA_DIR) + QDir::separator() + "testing_psd_ls.psd");

    QScopedPointer<KisDocument> doc(qobject_cast<KisDocument*>(KisPart::instance()->createDocument()));

    KisImportExportManager manager(doc.data());
    manager.setBatchMode(true);

    KisImportExportFilter::ConversionStatus status;
    QString s = manager.importDocument(sourceFileInfo.absoluteFilePath(), QString(),
                                       status);
    qDebug() << s;

    Q_ASSERT(doc->image());
}

void KisPSDTest::testTransparencyMask()
{
    QFileInfo sourceFileInfo(QString(FILES_DATA_DIR) + QDir::separator() + "sources/masks.psd");
    //QFileInfo sourceFileInfo("/home/dmitry/masks_multi.psd");

    Q_ASSERT(sourceFileInfo.exists());

    QScopedPointer<KisDocument> doc(qobject_cast<KisDocument*>(KisPart::instance()->createDocument()));

    KisImportExportManager manager(doc.data());
    manager.setBatchMode(true);

    KisImportExportFilter::ConversionStatus status;
    QString s = manager.importDocument(sourceFileInfo.absoluteFilePath(), QString(),
                                       status);
    // qDebug() << s;

    QVERIFY(doc->image());

    QImage result = doc->image()->projection()->convertToQImage(0, doc->image()->bounds());

    QVERIFY(TestUtil::checkQImageExternal(result, "psd_test", "transparency_masks", "kiki_single"));
}

QTEST_KDEMAIN(KisPSDTest, GUI)

#include "kis_psd_test.moc"
