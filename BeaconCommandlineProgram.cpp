#include "BeaconCommandlineProgram.h"

BeaconCommandlineProgram::BeaconCommandlineProgram(QString platform)
{
    if(platform=="auto"){
        if(BeaconPlatformInfo::isWindows)platform=QString("windows");
        else if(BeaconPlatformInfo::isMacos)platform=QString("macos");
        else if(BeaconPlatformInfo::isLinux)platform=QString("linux");
    }
    if(platform=="windows"){
        setProgram("cmd");
        prefixArgument << "/c";
    }
    else if(platform=="macos"){
        setProgram("open");
    }
    else if(platform=="linux"){
        //yakuake&guake not supported
        if(QFile::exists("/usr/bin/konsole")){
            bashBased=true;
            setProgram("konsole");
            prefixArgument << "--noclose" << "-e" ;
            prefixArgument << "bash" << "-c";
        }
        else if(QFile::exists("/usr/bin/gnome-terminal")){
            bashBased=true;
            manualHalt=true;
            prefixArgument << "--" << "bash" << "-c";
            setProgram("gnome-terminal");
        }
        else if(QFile::exists("/usr/bin/xfce4-terminal")){
            bashBased=true;
            manualHalt=true;
            prefixArgument << "-x" << "bash" << "-c";
            setProgram("xfce4-terminal");
        }
        else if(QFile::exists("/usr/bin/lxterminal")){
            bashBased=true;
            manualHalt=true;
            prefixArgument << "-e" << "bash" << "-c";
            setProgram("lxterminal");
        }
        else if(QFile::exists("/usr/bin/uxterm")){
            bashBased=true;
            manualHalt=true;
            prefixArgument << "-e" << "bash" << "-c";
            setProgram("uxterm");
        }
        else if(QFile::exists("/usr/bin/xterm")){
            bashBased=true;
            manualHalt=true;
            prefixArgument << "-e" << "bash" << "-c";
            setProgram("xterm");
        }
        //else
    }
}
void BeaconCommandlineProgram::setArguments(QStringList arg){
    QString t=arg.first();
    QStringList pathL=t.split("/");
    pathL.removeLast();
    QString path=pathL.join("/");
    this->program.setWorkingDirectory(path);

    QStringList arguments=prefixArgument;
    if(bashBased){
        QString mid;
        if(manualHalt)mid = QString("""%1;echo \"\n------Program Exited------\";sleep 2147483647""").arg(arg.first());
        else mid = QString("""%1;echo \"\n------Program Exited------\"").arg(arg.first());
        arguments.append(mid);
    }
    else arguments.append(arg);
    arguments.append(suffixArgument);
    this->program.setArguments(arguments);
}