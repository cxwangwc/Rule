#include "cxobjectinit.h"
#include "cxexternalobject.h"

QMap<QString, QMap<QString, QString> > CxObjectInitProperty::s_mapObjectProperty = QMap<QString, QMap<QString, QString> >();

QString CxObjectInitProperty::CappPath = "";
QString CxObjectInitProperty::TemplatePath = "";
QString CxObjectInitProperty::CxpSavePath = "";

void CxObjectInitProperty::initCXObject()
{
	s_mapObjectProperty.clear();
	QSettings st("./caxa.ini", QSettings::IniFormat);
	st.beginGroup("InitObject");
	QStringList objList = st.value("ObjectList").toStringList();
	foreach (QString obj, objList)
	{
		QStringList proList = st.value(obj).toStringList();
		QMap<QString, QString> propertMap;
		foreach (QString pro, proList)
		{
			QStringList propertyP = pro.split("--");
			if (propertyP.size() != 2)
				continue;
			propertMap.insert(propertyP.at(0), propertyP.at(1));
		}
		s_mapObjectProperty.insert(obj, propertMap);
	}
 	st.endGroup();
	CappPath = st.value("CappPath").toString();
	TemplatePath = st.value("TemplatePath").toString();
	CxpSavePath = st.value("CxpSavePath").toString();
}

CxExternalObject *CxObjectInitProperty::createObject(const QString &clsName)
{
	if (!s_mapObjectProperty.contains(clsName)) return 0;

	CxExternalObject *obj = new CxExternalObject();

	obj->clsName = clsName.toStdString();
	obj->objName = QString("new%1").arg(clsName).toStdString();
	QMap<QString, QString> properties =  s_mapObjectProperty.value(clsName);
	QMap<QString, QString>::iterator it = properties.begin();
	while (it != properties.end())
	{
		obj->attri.push_back(std::make_pair(it.key().toStdString(), std::make_pair(it.value().toStdString(), getDefaultValue(it.value()))));
		it++;
	}
	return obj;
}

std::string CxObjectInitProperty::getDefaultValue(const QString &init)
{
	if (init == "str")
	{
		return "";
	}
	else 
	{
		return "0";
	}
}