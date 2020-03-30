#include "UICustomCustomColorOperator.h"
#include <QDomElement>
#include <QFile>
#include <QTextStream>

const QString UICustomCustomColorOperator::m_cFileName = "./CustomColor.xml";

UICustomCustomColorOperator::UICustomCustomColorOperator(QObject* parent)
    :QObject(parent)
{

}

UICustomCustomColorOperator::~UICustomCustomColorOperator()
{

}

// 保存和加载颜色
void UICustomCustomColorOperator::saveCustomColors(const QList<QColor>& colors)
{
    QFile file(m_cFileName);
    if (file.exists())
        file.remove();

    QDomDocument doc;

    //写入xml头部
    QDomProcessingInstruction instruction; //添加处理命令
    instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
    doc.appendChild(instruction);

    // 添加颜色
    QDomElement rootElement = doc.createElement("CustomColor");
    doc.appendChild(rootElement);

    for (auto iter = colors.begin(); iter != colors.end(); ++iter)
    {
        QDomElement element = doc.createElement("Color");

        element.setAttribute("red", iter->red());
        element.setAttribute("green", iter->green());
        element.setAttribute("blue", iter->blue());

        rootElement.appendChild(element);
    }

    QFile xmlFile(m_cFileName);
    xmlFile.open(QFile::WriteOnly);
    QTextStream out_stream(&xmlFile);
    doc.save(out_stream, 4);
    xmlFile.close();
}

void UICustomCustomColorOperator::loadCustonColors(QList<QColor>& colors)
{
    colors.clear();

    // open file
    QFile file(m_cFileName);
    if (!file.open(QFile::ReadOnly))
        return;

    QDomDocument doc;
    if (!doc.setContent(&file))
    {
        file.close();
        return;
    }
    file.close();

    // root element
    QDomElement root = doc.documentElement();
    if (root.tagName() != "CustomColor")
        return;

    // nodeList
    QDomNodeList nodeList = root.childNodes();
    for (int i = 0; i < nodeList.count(); ++i)
    {
        QDomElement element = nodeList.at(i).toElement();

        QColor color;
        color.setRed(element.attribute("red").toInt());
        color.setGreen(element.attribute("green").toInt());
        color.setBlue(element.attribute("blue").toInt());

        colors.append(color);
    }
}
