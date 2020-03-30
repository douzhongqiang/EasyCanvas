#include "CustomTestWidget.h"
#include <QTabWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <QListView>
#include <QMenuBar>
#include <QProgressBar>
#include <QGroupBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTimeEdit>
#include <QDateEdit>
#include <QListWidget>
#include <QTreeWidget>

CustomTestWidget::CustomTestWidget(QWidget* parent)
    :CustomWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    m_pMenuBar = new QMenuBar;
    mainLayout->addWidget(m_pMenuBar);

    m_pTabWidget = new QTabWidget;
    mainLayout->addWidget(m_pTabWidget);

    initMenuBar();
    initCommonControls();
    initTableControls();
    initListControls();
}

CustomTestWidget::~CustomTestWidget()
{

}

void CustomTestWidget::initMenuBar(void)
{
    QMenu *fileMenu = new QMenu(tr("File"));
    fileMenu->addAction(tr("Open File"));
    fileMenu->addAction(tr("Save File"));
    fileMenu->addAction(tr("Exit"));
    m_pMenuBar->addMenu(fileMenu);

    QMenu *EditMenu = new QMenu(tr("Edit"));
    EditMenu->addAction(tr("Redo"));
    EditMenu->addAction(tr("Undo"));
    EditMenu->addAction(tr("Copy"));
    EditMenu->addAction(tr("Cut"));
    EditMenu->addAction(tr("Paste"));
    m_pMenuBar->addMenu(EditMenu);

    QMenu *otherMenu = new QMenu(tr("Other"));
    otherMenu->addAction(tr("Theme"));
    otherMenu->addAction(tr("Language"));
    otherMenu->addSeparator();
    otherMenu->addAction(tr("About"));
    m_pMenuBar->addMenu(otherMenu);
}

void CustomTestWidget::initCommonControls(void)
{
    QWidget *commonWidget = new QWidget;
    m_pTabWidget->addTab(commonWidget, tr("Common Controls"));
    QVBoxLayout* layout = new QVBoxLayout(commonWidget);

    // row1 - QLabel QLineEdit
    QHBoxLayout* topLayout = new QHBoxLayout;
    QLabel* tagWidget = new QLabel(tr("Label:"));
    QLineEdit* lineEdit = new QLineEdit(tr("Test lineEdit"));
    topLayout->addWidget(tagWidget);
    topLayout->addWidget(lineEdit);
    layout->addLayout(topLayout);

    // row2 - QPushButton
    QHBoxLayout *secondLayout = new QHBoxLayout;
    for (int i = 1; i <= 4; ++i)
    {
        QString buttonName = "%1_Button";
        buttonName = buttonName.arg(i);
        QPushButton* btn = new QPushButton;
        btn->setText(buttonName);
        //btn->setMinimumHeight(25);

        secondLayout->addWidget(btn);
    }
    layout->addLayout(secondLayout);

    // row3 - QComboBox
    QHBoxLayout *row3Layout = new QHBoxLayout;
    QComboBox* comboBox1 = new QComboBox;
    comboBox1->setView(new QListView(comboBox1));
    comboBox1->addItem(tr("Test1"));
    comboBox1->addItem(tr("Test2"));
    comboBox1->addItem(tr("Test3"));
    comboBox1->addItem(tr("Test4"));
    row3Layout->addWidget(comboBox1);
    layout->addLayout(row3Layout);

    // row4 - QSlider and QProgressBar
    QHBoxLayout* row4Layout = new QHBoxLayout;
    QSlider *slider = new QSlider(Qt::Horizontal);
    slider->setMaximum(100);
    slider->setMinimum(0);
    slider->setValue(50);
    row4Layout->addWidget(slider);
    QProgressBar *progressBar = new QProgressBar;
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    progressBar->setValue(50);
    QObject::connect(slider, &QSlider::valueChanged, progressBar, &QProgressBar::setValue);
    row4Layout->addWidget(progressBar);
    layout->addLayout(row4Layout);

    // row5 - checkbox and radiobutton
    // checkBox
    QGroupBox *checkBoxGroupBox = new QGroupBox;
    checkBoxGroupBox->setTitle(tr("CheckBox"));
    QHBoxLayout *checkBoxLayout = new QHBoxLayout(checkBoxGroupBox);
    QCheckBox* checkBox1 = new QCheckBox(tr("CheckBox1"));
    QCheckBox* checkBox2 = new QCheckBox(tr("CheckBox2"));
    checkBoxLayout->addWidget(checkBox1);
    checkBoxLayout->addWidget(checkBox2);
    // add Widgets
    QHBoxLayout *row5Layout = new QHBoxLayout;
    row5Layout->addWidget(checkBoxGroupBox);
    layout->addLayout(row5Layout);
    // radioButton
    QGroupBox *radioButtonGroupBox = new QGroupBox;
    radioButtonGroupBox->setTitle(tr("RadioButton"));
    QHBoxLayout *radioButtonLayout = new QHBoxLayout(radioButtonGroupBox);
    QRadioButton* radioButton1 = new QRadioButton(tr("RadioButton1"));
    QRadioButton* radioButton2 = new QRadioButton(tr("RadioButton2"));
    radioButtonLayout->addWidget(radioButton1);
    radioButtonLayout->addWidget(radioButton2);
    row5Layout->addWidget(radioButtonGroupBox);

    // row6
    QHBoxLayout* row6Layout = new QHBoxLayout;
    QDoubleSpinBox* doubleSpinBox = new QDoubleSpinBox;
    QSpinBox* spinBox = new QSpinBox;
    QDateEdit *dateEdit = new QDateEdit;
    QTimeEdit* timeEdit = new QTimeEdit;
    row6Layout->addWidget(doubleSpinBox);
    row6Layout->addWidget(spinBox);
    row6Layout->addWidget(dateEdit);
    row6Layout->addWidget(timeEdit);
    layout->addLayout(row6Layout);

    // row7 - QTextEdit
    QHBoxLayout* row7Layout = new QHBoxLayout;
    QTextEdit* edit = new QTextEdit;
    row7Layout->addWidget(edit);
    layout->addLayout(row7Layout);
}

void CustomTestWidget::initTableControls(void)
{
    QTableWidget* tableWidget = new QTableWidget;
    m_pTabWidget->addTab(tableWidget, tr("TableWidget"));

    tableWidget->setRowCount(100);
    tableWidget->setColumnCount(5);
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    QStringList heads;
    heads << tr("ID") << tr("Name") << tr("Sex") << tr("Age") << tr("Score");
    tableWidget->setHorizontalHeaderLabels(heads);

    int ids[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    QString names[10] = { "douzhq", "xiaoming", "xiaohong", "xiaozhao", "xiaoli", \
        "douzhq", "xiaoming", "xiaohong", "xiaozhao", "xiaoli" };
    QString sex[10] = { "man", "man", "female", "man", "man", "female", "man", "man", "female", "man" };
    int ages[10] = { 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
    int Score[10] = { 100, 99, 98, 97, 96, 95, 94, 93, 92, 91 };
    
    for (int i = 0; i < 100; ++i)
    {
        tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(ids[i % 10])));
        tableWidget->setItem(i, 1, new QTableWidgetItem(names[i % 10]));
        tableWidget->setItem(i, 2, new QTableWidgetItem(sex[i % 10]));
        tableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(ages[i % 10])));
        tableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(Score[i % 10])));
    }
}

void CustomTestWidget::initListControls(void)
{
    QWidget* w = new QWidget;
    w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_pTabWidget->addTab(w, "ListWidget");

    QHBoxLayout* layout = new QHBoxLayout(w);
    QListWidget* listWidget = new QListWidget;
    listWidget->addItem(new QListWidgetItem(tr("ListWidgetItem1")));
    listWidget->addItem(new QListWidgetItem(tr("ListWidgetItem2")));
    listWidget->addItem(new QListWidgetItem(tr("ListWidgetItem3")));
    listWidget->addItem(new QListWidgetItem(tr("ListWidgetItem4")));
    layout->addWidget(listWidget);

    QTreeWidget* treeWidget = new QTreeWidget;
    layout->addWidget(treeWidget);
    QTreeWidgetItem* topItem1 = new QTreeWidgetItem();
    topItem1->setText(0, "topLevelItem1");
    topItem1->addChild(new QTreeWidgetItem(QStringList("Top1_Child1")));
    topItem1->addChild(new QTreeWidgetItem(QStringList("Top1_Child2")));
    treeWidget->addTopLevelItem(topItem1);

    QTreeWidgetItem* topItem2 = new QTreeWidgetItem();
    topItem2->setText(0, "topLevelItem2");
    topItem2->addChild(new QTreeWidgetItem(QStringList("Top2_Child1")));
    topItem2->addChild(new QTreeWidgetItem(QStringList("Top2_Child2")));
    treeWidget->addTopLevelItem(topItem2);
}
