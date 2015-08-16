#include <QApplication>
#include <QCommandLineParser>
#include <gui/tMainForm.h>

#include "model/tModel.h"
#include "model/tModelIO.h"
#include "model/export/tExporterTool.h"


#include <QtDebug>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    QApplication::setApplicationName("jrCAd");
    QApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("jrCad options:");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addPositionalArgument("jrFile", "geometry file to open");

    QCommandLineOption noguiOption("nogui","launch without gui, just process input files.");
    parser.addOption(noguiOption);

    // An option with a value
    QCommandLineOption exportOption("export","export model to  file.","file");
    parser.addOption(exportOption);

    QCommandLineOption selectOption("select","element names to be selected.","regular expression which matches initially selected element names");
    parser.addOption(selectOption);

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();

    bool noGui = parser.isSet(noguiOption);

    if (noGui){
        if (args.count()){
            tModel *m = new tModel(NULL);
            m->addOwner(m);

            tModelIO io(m);

            foreach (QString jrFile, args) {
                qDebug() << "open jr file: " << jrFile;
                io.fromXmlFile(jrFile);
            }

            tSelectionSet* selectionSet = NULL;
            if (parser.isSet(selectOption)){
                QRegularExpression regExp (parser.value(selectOption));
                qDebug() << "select elements: " << regExp.pattern();

                selectionSet = new tSelectionSet(m, true,-1);
                selectionSet->setElements(m->elementsByName(regExp));
                qDebug() << "found:";
                for (int i=0; i<selectionSet->nElements(); i++){
                    iElement *iFace = selectionSet->element(i)->intrface();
                    QString out = "  " + iFace->name() + " (" + iFace->type() +")";
                    qDebug() << out.toLatin1().data();
                }
            }

            if (parser.isSet(exportOption)){
                QString exportTo = parser.value(exportOption);
                qDebug() << "export to: " << exportTo;
                tExporterTool exporter(m, selectionSet);
                if (exporter.exportToFile(exportTo)) {
                    qDebug() << "  done";
                } else {
                    qDebug() << "  export failed";
                }
            }
            if (selectionSet){
                selectionSet->releaseOwner(m);
            }
            m->releaseOwner(NULL);

        } else {

        }
        return 0;
    } else {
        tMainForm *MainForm;
        MainForm = new tMainForm;
        if (args.count()){
            QString jrFile = args[0];
            tModel *m = MainForm->createEmptyModel(jrFile);

            tModelIO io(m);
            foreach (QString jrFile, args) {
                qDebug() << "open jr file: " << jrFile;
                io.fromXmlFile(jrFile);
            }

            tSelectionSet* selectionSet = NULL;
            if (parser.isSet(selectOption)){
                QRegularExpression regExp (parser.value(selectOption));
                //qDebug() << "select elements: " << regExp.pattern();

                selectionSet = MainForm->getActiveSelectionSet();

                selectionSet->setElements(m->elementsByName(regExp));
                //qDebug() << "found:";
                //for (int i=0; i<selectionSet->nElements(); i++){
                //    iElement *iFace = selectionSet->element(i)->intrface();
                //    QString out = "  " + iFace->name() + " (" + iFace->type() +")";
                //    qDebug() << out.toLatin1().data();
               // }
            }

            if (parser.isSet(exportOption)){
                QString exportTo = parser.value(exportOption);
                //qDebug() << "export to: " << exportTo;
                tExporterTool exporter(m, selectionSet);
                if (exporter.exportToFile(exportTo)) {
                //    qDebug() << "  done";
                //} else {
                //    qDebug() << "  export failed";
                }
            }
        }

        MainForm->show();
        return app.exec();
    }

}
