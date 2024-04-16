#!/bin/bash

# 判断参数列表是否为空
if [ "$#" -eq 0 ]; then
    echo "错误：没有提供文件名。"
    exit 1
fi

# 循环遍历所有的输入参数
for file in "$@"
do
    # 检查文件是否存在
    if [ -f "$file" ]; then
        # 打印文件名到output.txt
        echo "\n\n====== 文件：$file ======" >> output.txt
        # 把文件内容追加到output.txt
        cat "$file" >> output.txt
    else
        echo "错误：文件 '$file' 不存在."
    fi
done

echo "操作完成，请查看 output.txt"