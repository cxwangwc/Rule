1、常用数学函数介绍
　　  
计算毛坯库子规则：
	函数名称：calIncomeRate(excelName, Quantity, L, W, H, PRO_TYPE='圆角矩形'):	
	现在只有圆角矩形的计算规则，其他形状的未实现
	函数参数:
		excelName	：文本类型，保存毛坯库的excel文件,excel文件有四列，分别为名称、长、宽、厚
		Quantity	：数量、输入参数
		L			：产品长度
		W			：产品宽度
		H			：产品厚度
		PRO_TYPE	：产品类型
	返回值是一个列表，有三项：
		第一项是一个列表，里面的子项也是一个列表，子项数由excel文件毛坯个数决定，每一个子项由
	毛坯名称(即编号)、毛坯长、毛坯宽、毛坯厚、毛坯不反向所得率、毛坯反向所得率六项组成
		第二项也是一个列表，返回的是所得率最高的毛坯尺寸及对应的所得率，由六项组成，分别为
	毛坯名称(即编号)、毛坯长、毛坯宽、毛坯厚、毛坯不反向所得率、毛坯反向所得率
		第三项是一个列表，返回的是这种类型的产品推荐使用的毛坯尺寸，里面有四项，分别为：
	毛坯长、毛坯宽、毛坯厚、所得率


	使用实例：
	假如保存毛坯的excel文件为 
		excelName = 'd:\data\blank.xlsx'
		L = 4.75
		W = 3.75
		H = 0.75
		Quantity = 2000
	
	则要获取毛坯库里所得率最高的毛坯名称blankName、毛坯长度blankWidth、毛坯宽度blankHeight、毛坯厚度blankThick、毛坯不反向所得率blankRatio、
	毛坯反向所得率blankRatioInvert，
	需要这样调用这个函数：
		retParam = calIncomeRate(excelName, Quantity, L, W, H)
		blankName = retParam[1][0]
		blankWidth = retParam[1][1]
		blankHeight = retParam[1][2]
		blankThick = retParam[1][3]
		blankRatio = retParam[1][4]
		blankRatioInvert = retParam[1][5]
		如果您还需要显示推荐使用的毛坯，毛坯长度blankWidthSuggest、毛坯宽度blankHeightSuggest、毛坯厚度blankThickSuggest、
	推荐毛坯所得率blankRatioSuggest
	再获取完实际库存里最高所得率的之后，可以再获取推荐毛坯的尺寸
		blankWidthSuggest  = retParam[2][0]
		blankHeightSuggest  = retParam[2][1]
		blankThickSuggest  = retParam[2][2]
		blankRatioSuggest = retParam[2][3]
		
	
1.1、Number-theoretic and representation functions

	math.ceil(x) 
	返回数的上入整数

	math.copysign(x, y) 
	返回x的绝对值，但是符号是y的符号
	copysign(1.0, -0.0)返回 -1.0.

	math.fabs(x) 
	返回x的绝对值

	math.factorial(x) 
	Return x factorial. Raises ValueError if x is not integral or is negative.

	math.floor(x) 
	返回数的下舍整数

	math.fmod(x, y) 
	返回x%y

	math.fsum(iterable) 
	返回序列iterable的和

	round(number[, ndigits])
	根据给定的精度对数字进行四舍五入 如:round(3.1415, 2) 返回3.14

1.2、Power and logarithmic functions

	math.exp(x) 
	e的x次幂

	math.log(x[, base]) 
	一个参数时，返回e的对数，两个参数时，返回base的对数
	With one argument, return the natural logarithm of x (to base e).


	math.log10(x) 
	返回以10为底的x的对数

	math.pow(x, y) 
	返回x的y次幂

	math.sqrt(x) 
	x的平方根

1.3、Trigonometric functions
	math.acos(x) 
	Return the arc cosine of x, in radians.

	math.asin(x) 
	Return the arc sine of x, in radians.

	math.atan(x) 
	Return the arc tangent of x, in radians.

	math.atan2(y, x) 
	Return atan(y / x), in radians. The result is between -pi and pi. The vector in the plane from the origin to point (x, y) makes this angle with the positive X axis. The point of atan2() is that the signs of both inputs are known to it, so it can compute the correct quadrant for the angle. For example, atan(1) and atan2(1, 1) are both pi/4, but atan2(-1, -1) is -3*pi/4.

	math.cos(x) 
	Return the cosine of x radians.

	math.hypot(x, y) 
	Return the Euclidean norm, sqrt(x*x + y*y). This is the length of the vector from the origin to point (x, y).

	math.sin(x) 
	Return the sine of x radians.

	math.tan(x) 
	Return the tangent of x radians.

1.4、Angular conversion
	math.degrees(x) 
	把角度转换为弧度

	math.radians(x) 
	把弧度转换为角度

1.5、Hyperbolic functions
	Hyperbolic functions are analogs of trigonometric functions that are based on hyperbolas instead of circles.

	math.acosh(x) 
	Return the inverse hyperbolic cosine of x.

	math.asinh(x) 
	Return the inverse hyperbolic sine of x.

	math.atanh(x) 
	Return the inverse hyperbolic tangent of x.

	math.cosh(x) 
	Return the hyperbolic cosine of x.

	math.sinh(x) 
	Return the hyperbolic sine of x.

	math.tanh(x) 
	Return the hyperbolic tangent of x.


1.6、Constants
	math.pi 
	The mathematical constant π = 3.141592..., to available precision.

	math.e 
	The mathematical constant e = 2.718281..., to available precision.

