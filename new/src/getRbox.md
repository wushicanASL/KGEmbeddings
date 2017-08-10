# 生成rbox文件
## 编译
`make getRbox`
## 运行
`./gerRbox.out [DataSet]`
其中`DataSet`的取值为`{ FB15k, WN18, WN11, FB13, FB40k }`

# 输出格式
程序会在当前目录下输出`[DataSet].rbox`文件，文件每一行为一条RBox规则，具体可能是：
1. `symm r1` 表示`r1`具有对称性质
2. `tran r1` 表示`r1`具有传递性质
3. `refl r1` 表示`r1`具有自反性质
4. `asym r1` 表示`r1`具有反对称性质
5. `irre r1` 表示`r1`具有反自反性质
6. `func r1` 表示`r1`具有函数性质
7. `ifun r1` 表示`r1`具有逆函数性质
8. `inve r1 r2` 表示`r1`和`r2`之间存在互逆性质
9. `disj r1 r2` 表示`r1`和`r2`之间存在互斥性质
10. `subr r1 r2` 表示`r1`和`r2`之间存在包含性质，`(X, r1, Y) -> (X, r2, Y)`
11. `subc r1 r2 r3` 表示`r1`,`r2`和`r3`之间存在`(X, r1, Y) and (Y, r2, Z) -> (X, r3, Z)`