build.sh用来编译cli-api.cpp 内部命令行未采用boost option库

build_new.sh用来编译cli-api-new.cpp 内部命令行采用boost option库

cli-api使用介绍
使用编译后的可执行文件
执行方式如下
（[]内为前面标识符后对应要输入的内容 {}中为可选项）
压缩
./cli-api compress -tb [tablename] {-h [host] -u [user] -p [passwd] -db [database] -f [filename] -t [time] -d [distance]} 
解压
./cli-api restore -f [filename] {-h [host] -u [user] -p [passwd] -db [database] -tb [tablename] -t [time]} 
帮助信息
./cli-api help
细项对应信息解释
compress、restore和help三个命令必须为第一个输入参数，而后的分支指令对顺序没有要求
对compress -tb [tablename]必须要有
对restore -f [filename]必须要有
  -h, [host]         mysql host
  -u, [user]         mysql user
  -p, [passwd]       mysql password
  -db,[database]     traces database
  -tb,[table]        traces table[must enter]
  -f, [filename]     save traces filename
  -t, [time]         compression time interval
  -d, [distance]     compression limit distance


  cli-api-new使用介绍
  all options:
  -h [ --help ]         Help message
  -c [ --compress ]     Compression commands
  -r [ --restore ]      Restore commands
  --host arg            MySQL host
  -u [ --user ] arg     MySQL user
  -p [ --passwd ] arg   MySQL passwd
  -d [ --database ] arg MySQL use database
  -s [ --sheet ] arg    MySQL compress or restore table
  -f [ --filename ] arg the file name which want to save or read
  -t [ --time ] arg     traces time interval
  -l [ --limit ] arg    compression limit distance
  与cli-api细项解释基本一致
  因缩写冲突将table改为了sheet，将distance改为limit
  在cli-api-new中对指令顺序没有特殊的要求，但对于compress指令 -s [tablename]是不可缺少的 对于restore -f [filename]是不可缺少的