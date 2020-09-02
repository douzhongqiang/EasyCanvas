#include "ItemAttributeChangedCmd.h"
#include "NDAttributeBase.h"

ItemAttributeChangedCmd::ItemAttributeChangedCmd(const QList<NDAttributeBase*>& pAttributeList, \
                                                 const QVariant& value, bool canRun)
    :m_isCanRun(canRun)
{
    QStringList attrNames;

    m_newValueList.push_back(value);
    for (auto iter = pAttributeList.begin(); iter != pAttributeList.end(); ++iter)
    {
        attrNames << (*iter)->getFullName();
        m_pAttributeList.push_back(QPointer<NDAttributeBase>(*iter));
    }

    QString nameString = "Changed Attribute [%1] To %2";
    nameString = nameString.arg(attrNames.join(",")).arg(value.toString());
    this->setText(nameString);
}

ItemAttributeChangedCmd::ItemAttributeChangedCmd(const QList<NDAttributeBase*>& pAttributeList, const QVector<QVariant>& values, bool canRun)
    :m_newValueList(values)
    ,m_isCanRun(canRun)
{
    QStringList attrNames;
    QStringList valueStrings;

    for (auto iter = pAttributeList.begin(); iter != pAttributeList.end(); ++iter)
    {
        attrNames << (*iter)->getFullName();
        m_pAttributeList.push_back(QPointer<NDAttributeBase>(*iter));
    }

    for (auto iter = values.begin(); iter != values.end(); ++iter)
        valueStrings << iter->toString();

    QString nameString = "Changed Attribute [%1] To [%2]";
    nameString = nameString.arg(attrNames.join(",")).arg(valueStrings.join(","));
    this->setText(nameString);
}

ItemAttributeChangedCmd::~ItemAttributeChangedCmd()
{

}

void ItemAttributeChangedCmd::undo()
{
    int count = 0;
    for (auto iter = m_pAttributeList.begin(); iter != m_pAttributeList.end(); ++iter)
    {
        QVariant value = m_oldValueList[count++];

        QPointer<NDAttributeBase> pointer = *iter;
        if (pointer.isNull())
            continue;

        pointer->setValue(value);
    }
}

void ItemAttributeChangedCmd::redo()
{
    int count = 0;
    bool isOldEmpty = m_oldValueList.isEmpty();
    for (auto iter = m_pAttributeList.begin(); iter != m_pAttributeList.end(); ++iter)
    {
        QPointer<NDAttributeBase> pointer = *iter;

        if (pointer.isNull())
            continue;

        if (m_isCanRun)
            pointer->setValue(m_newValueList[count++]);

        if (isOldEmpty)
            m_oldValueList << pointer->getLastValue();
    }

    m_isCanRun = true;
}
