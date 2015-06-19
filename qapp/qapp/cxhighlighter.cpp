#include "cxhighlighter.h"

CxHighlighter::CxHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    CxHighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;

	keywordPatterns << "\\bint\\b" << "\\bclass\\b" << "\\bfloat\\b"
		<< "\\bstr\\b" << "\\bdef\\b" << "\\bif\\b"
		<< "\\bpass\\b" << "\\belse\\b" << "\\belif\\b"
		<< "\\breturn\\b" << "\\import\\b" << "\\bfrom\\b"<< "\\bTrue\\b"<< "\\bFalse\\b"
		<< "\\bglobal\\b" << "\\bnot\\b" << "\\bin\\b" << "\\bfor\\b" << "\\bwhile\\b"
		<< "\\bpi\\b" << "\\band\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
	rule.pattern = QRegExp("#[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);
    multiLineCommentFormat.setForeground(Qt::red);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

	singleQuotationFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("'.*\'");
	rule.format = singleQuotationFormat;
	highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("\\b'''");
    commentEndExpression = QRegExp("\\b'''");
}

void CxHighlighter::highlightBlock(const QString &text)
{
    foreach (const CxHighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}