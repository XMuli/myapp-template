#include "settingnavui.h"
#include "ui_settingnavui.h"
#include "ui_sub_general.h"
#include "ui_sub_update.h"
#include "ui_sub_about.h"
#include <QDebug>

SettingNavUI::SettingNavUI(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingNavUI)
    , uiSubGeneral(new Ui::SubGeneral)
    , uiSubUpdate(new Ui::SubUpdate)
    , uiSubAbout(new Ui::SubAbout)
{
    ui->setupUi(this);
    initUI();
}

SettingNavUI::~SettingNavUI()
{
    delete ui;
    delete uiSubGeneral;
    delete uiSubUpdate;
    delete uiSubAbout;
}

void SettingNavUI::onItemClicked(QTreeWidgetItem *item, int column)
{
    qDebug() << "item:" << item;
}

void SettingNavUI::initUI()
{
    // 单独的 .ui 添加到 TabWidget 子对象中
    uiSubGeneral->setupUi(ui->generalDlg);
    uiSubUpdate->setupUi(ui->updateDlg);
    uiSubAbout->setupUi(ui->aboutDlg);

    auto& tree = ui->treeWidget;
    auto& stacked = ui->stackedWidget;
    QTreeWidgetItem* itemGeneral = new QTreeWidgetItem(tree, QStringList(tr("General")));
    QTreeWidgetItem* itemInterface = new QTreeWidgetItem(tree, QStringList(tr("Interface")));
    // QTreeWidgetItem* itemOCR = new QTreeWidgetItem(tree, QStringList(tr("OCR")));
        QTreeWidgetItem* itemExtractText = new QTreeWidgetItem(tree, QStringList(tr("Extract text")));
        QTreeWidgetItem* itemImageTrans = new QTreeWidgetItem(tree, QStringList(tr("Image translation")));
    QTreeWidgetItem* itemOutput = new QTreeWidgetItem(tree, QStringList(tr("Output")));
    QTreeWidgetItem* itemShortcuts = new QTreeWidgetItem(tree, QStringList(tr("Shortcuts")));
    QTreeWidgetItem* itemInfo = new QTreeWidgetItem(tree, QStringList(tr("Info")));
        QTreeWidgetItem* itemUpdates = new QTreeWidgetItem(itemInfo, QStringList(tr("Updates")));
        QTreeWidgetItem* itemAbout = new QTreeWidgetItem(itemInfo, QStringList(tr("About")));

    tree->expandAll();

    QMap<QTreeWidgetItem*, int> itemToPageMap;
    itemToPageMap[itemGeneral] = stacked->indexOf(ui->generalDlg); //stacked->addWidget(ui->generalDlg);
    itemToPageMap[itemUpdates] = stacked->indexOf(ui->updateDlg);
    itemToPageMap[itemAbout] = stacked->indexOf(ui->aboutDlg);

    // 遍历并打印 QMap 的内容
    for (auto it = itemToPageMap.begin(); it != itemToPageMap.end(); ++it) {
        QTreeWidgetItem* item = it.key();
        int pageIndex = it.value();

        // 打印指针地址和页面索引
        qDebug() << "QTreeWidgetItem*" << item << "Page Index:" << pageIndex << "Item Text:" << item->text(0);
    }

    // 关联信号和槽，切换页面
    QObject::connect(ui->treeWidget, &QTreeWidget::currentItemChanged, [&, itemToPageMap](QTreeWidgetItem *current, QTreeWidgetItem *previous) {
        qDebug() << "current:" << current << itemToPageMap[current];
        if (current && itemToPageMap.contains(current)) {
            ui->stackedWidget->setCurrentIndex(itemToPageMap[current]);
        }
    });

}
