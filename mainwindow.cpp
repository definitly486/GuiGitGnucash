#include "mainwindow.h"
#include "ui_GuiGitGnucash.h"
#include <QFile>
#include <QMessageBox>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Убедитесь, что в UI для lineEdit установлен режим Password
    ui->lineEdit->setEchoMode(QLineEdit::Password);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::appendMessage(const QString &message)
{
    ui->textEdit->append(message);
}


bool copyFile(const QString &sourcePath, const QString &destinationPath, MainWindow *mainWin)
{
    QFile sourceFile(sourcePath);
    if (QFile::exists(destinationPath)) {
        QFile::remove(destinationPath);
    }

    if (sourceFile.copy(destinationPath)) {
        return true;
    } else {
        QString message = QObject::tr("Не удалось скопировать файл: %1").arg(sourceFile.errorString());
        QMessageBox::warning(nullptr, "Ошибка", message);
        if (mainWin) mainWin->appendMessage(message);
        return false;
    }
}

// шифрование файла
bool encryptFileWithPassword(const QString &inputFile, const QString &outputFile, const QString &password, MainWindow *mainWin)
{
    QString program = "gpg";

    QStringList arguments;
    arguments << "--batch"
              << "--yes"
              << "--passphrase" << password
              << "-c"
              << "-o" << outputFile
              << inputFile;

    QProcess process;
    process.start(program, arguments);
    if (!process.waitForFinished(30000)) {
        QString msg = "GPG процесс не завершился вовремя.";
        if (mainWin) mainWin->appendMessage(msg);
        return false;
    }

    if (process.exitStatus() != QProcess::NormalExit || process.exitCode() != 0) {
        QString errorMsg = "Ошибка шифрования: " + QString(process.readAllStandardError());
        if (mainWin) mainWin->appendMessage(errorMsg);
        return false;
    }

    if (mainWin) mainWin->appendMessage("Файл успешно зашифрован");
    return true;
}

// выполнение git команд
bool runGitCommands(const QString &repoPath, MainWindow *mainWin)
{
    QProcess process;
    process.setWorkingDirectory(repoPath);

    // Настройка user.email
    process.start("git", QStringList() << "config" << "--global" << "user.email" << "you@example.com");
    if (!process.waitForFinished(30000) || process.exitCode() != 0) {
        QString errorMsg = "Ошибка настройки user.email: " + QString(process.readAllStandardError());
        if (mainWin) mainWin->appendMessage(errorMsg);
        return false;
    }

    // Настройка user.name
    process.start("git", QStringList() << "config" << "--global" << "user.name" << "Ваше Имя");
    if (!process.waitForFinished(30000) || process.exitCode() != 0) {
        QString errorMsg = "Ошибка настройки user.name: " + QString(process.readAllStandardError());
        if (mainWin) mainWin->appendMessage(errorMsg);
        return false;
    }

    // git add --all
    process.start("git", QStringList() << "add" << "--all");
    if (!process.waitForFinished(30000) || process.exitCode() != 0) {
        QString errorMsg = "Ошибка git add --all: " + QString(process.readAllStandardError());
        if (mainWin) mainWin->appendMessage(errorMsg);
        return false;
    }

    // git add .
    process.start("git", QStringList() << "add" << ".");
    if (!process.waitForFinished(30000) || process.exitCode() != 0) {
        QString errorMsg = "Ошибка git add .: " + QString(process.readAllStandardError());
        if (mainWin) mainWin->appendMessage(errorMsg);
        return false;
    }

    // git commit
    process.start("git", QStringList() << "commit" << "-m" << "Автоматический коммит");
    if (!process.waitForFinished(30000) || process.exitCode() != 0) {
        QString errorMsg = "Ошибка git commit: " + QString(process.readAllStandardError());
        if (mainWin) mainWin->appendMessage(errorMsg);
        return false;
    }

    // git push
    process.start("git", QStringList() << "push" << "git@github.com2:xinitronix/gnucash.git");
    if (!process.waitForFinished(30000) || process.exitCode() != 0) {
        QString errorMsg = "Ошибка git push: " + QString(process.readAllStandardError());
        if (mainWin) mainWin->appendMessage(errorMsg);
        return false;
    }

    if (mainWin) mainWin->appendMessage("Все команды успешно выполнены.");
    return true;
}
void MainWindow::on_pushButton_clicked()
{
    QString password = ui->lineEdit->text();

    if (password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пароль не введен");
        return;
    }

    // Передача указателя на текущий объект
    if (!encryptFileWithPassword("/ntfs-2TB/gnucash/definitly.gnucash",
                                 "/ntfs-2TB/gnucash/definitly.gnucash.gpg",
                                 password,
                                 this))
    {
        // Обработка ошибки
    }

    if (!copyFile("/ntfs-2TB/gnucash/definitly.gnucash.gpg",
                  "/ntfs-2TB/GIT/gnucash/definitly.gnucash.gpg",
                  this))
    {
        // Обработка ошибки
    }

    if (!runGitCommands("/ntfs-2TB/GIT/gnucash", this))
    {
        // Обработка ошибки
    }
}

void MainWindow::on_textEdit_undoAvailable(bool b)
{

}


void MainWindow::on_lineEdit_cursorPositionChanged()
{

}

