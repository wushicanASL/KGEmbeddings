#test_main参数说明

* -embedding
    指定训练模型
    默认取值：TransE
    可选取值：TransE TransH TransD

* -dataset
    指定数据集
    默认取值：WN18
    可选取值：WN18 FB15k WN11 FB13 FB40k
    注意：程序会根据数据集自动选择测试类型。WN18和FB15k会进行链接预测实验，WN11和FB13会进行分类实验，FB40k不会进行实验。

* -sampling
    指定采样方法
    默认取值：unif
    可选取值：unif bern update

* -dim
    指定训练模型维数
    默认取值：100
    可选取值：非0 unsigned int

* -nepoch
    指定梯度下降迭代次数
    默认取值：1000
    可选取值：非0 unsigned int

* -threads
    指定训练以及链接预测的并发线程数
    默认取值：4
    可选取值：非0 unsigned int
    建议取值：CPU核心数

* -margin
    指定训练阈值
    默认取值：1
    可选取值：float
    建议取值：1

* -rate
    梯度下降学习率
    默认取值：0.001
    可选取值：float

* -ext
    已训练向量的输入文件扩展名
    默认取值：空（即无输入文件）

* -mode
    程序模式
    默认取值：retrain
    可选取值：retrain - 无视输入文件，随机初始化所有向量，使用train集和update集重新训练
             basetrain - 无视输入文件，随机初始化所有向量，仅使用train集重新训练
             update - 必须有输入文件，读取输入文件中的向量，使用train集和update集更新训练
             testonly - 必须有输入文件，不进行训练，仅对输入的向量进行测试实验

* -output
    测试输出间隔
    默认取值：0（不进行测试）
    可选取值：unsigned int，例如取2则会在第2 4 6 8 ...轮训练迭代后进行测试并输出结果

* -silence
    静默模式，不再输出每一轮的loss估值
    可选取值：无
