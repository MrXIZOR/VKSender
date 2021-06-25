#include "finishedwidget.h"
#include "ui_finishedwidget.h"

FinishedWidget::FinishedWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FinishedWidget)
{
    ui->setupUi(this);

    ui->listWidget->layout()->setAlignment(Qt::AlignTop);
}

void FinishedWidget::setFetcher(const std::shared_ptr<Fetcher> fetcher)
{
    mFetcher = fetcher;
}

void FinishedWidget::addFinishedItem(const SendingResult &result)
{
    FinishedListWidgetItem *item = new FinishedListWidgetItem(this);
    item->setFetcher(mFetcher);
    connect(item, &FinishedListWidgetItem::finishedListWidgetItemReleased,
            this, &FinishedWidget::finishedListWidgetItemReleased);
    connect(item, &FinishedListWidgetItem::backToWaiting,
            this, &FinishedWidget::onAbortionFinished);
    connect(item, &FinishedListWidgetItem::showFinishedWidget,
            this, &FinishedWidget::showWidget);
    item->setSendingResult(result);
    ui->listWidget->layout()->addWidget(item);
}

void FinishedWidget::onAbortionFinished(MessagePack message)
{
    sender()->deleteLater();
    emit abortionFinished(message);
}

FinishedWidget::~FinishedWidget()
{
    delete ui;
}
