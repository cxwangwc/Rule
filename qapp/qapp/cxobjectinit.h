#ifndef CXOBJECTINIT_H
#define CXOBJECTINIT_H

#include <QMap>
#include <QSettings>

class CxExternalObject;

#define CLS_FUNCTIONS
#define CLS_MEMBERS

class CxObjectInitProperty
{
public CLS_FUNCTIONS:
	static void initCXObject();
	static CxExternalObject *createObject(const QString &clsName);
	static std::string getDefaultValue(const QString &);

public CLS_MEMBERS:
	static QMap<QString, QMap<QString, QString> > s_mapObjectProperty;
	static QString CappPath;
	static QString TemplatePath;
	static QString CxpSavePath;
};

#endif