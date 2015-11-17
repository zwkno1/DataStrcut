#include "controlwidget.h"
#include "ui_controlwidget.h"

ControlWidget::ControlWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget)
{
    ui->setupUi(this);
    connect(ui->pushButton_insert, &QPushButton::released, this, &ControlWidget::onInsert);
    connect(ui->pushButton_remove, &QPushButton::released, this, &ControlWidget::onRemove);
}

ControlWidget::~ControlWidget()
{
    delete ui;
}

void ControlWidget::onInsert()
{
    insert(ui->spinBox->value());
}

void ControlWidget::onRemove()
{
    remove(ui->spinBox->value());
    ui->spinBox->setValue(ui->spinBox->value() + 1);
}
