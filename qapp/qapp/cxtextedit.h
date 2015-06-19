#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QTextEdit>
#include <vector>
#include <string>

class QCompleter;

class CxTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    CxTextEdit(QWidget *parent = 0, const QStringList &keywordsList = QStringList());
    ~CxTextEdit();

    void setCompleter(const QStringList &keyList);
    QCompleter *completer() const;

	void setParamCompleter(const std::vector<std::string> &);

signals:
	void runFile(const std::string &fileName);
	void selectedPos(const QString &);

protected:
	void contextMenuEvent(QContextMenuEvent *e);
	void mousePressEvent(QMouseEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);

private slots:
    void insertCompletion(const QString &completion);
	void runSimpleString();
	void openFile();

private:
    QString textUnderCursor() const;
	void functionsFromFile(const QString& fileName);

private:
    QCompleter *m_completer;
	QStringList m_functionList;
};

#endif // TEXTEDIT_H

