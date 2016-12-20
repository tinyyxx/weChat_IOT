<?php
if ($_GET['token'] == "arduino") {//可以改token,这相当于密码，在Arduino端改成相应的值即可
	$con = mysql_connect(SAE_MYSQL_HOST_M.':'.SAE_MYSQL_PORT,SAE_MYSQL_USER,SAE_MYSQL_PASS);
	mysql_select_db("app_wechatiot", $con);//要改成相应的数据库名
	$result = mysql_query("SELECT * FROM sensor");
	while($arr = mysql_fetch_array($result)){//找到需要的数据的记录，并读出状态值
		if ($arr['ID'] == 1) {
			$state = $arr['state'];
		}
	}
    
    mysql_close($con);
    //echo "abcdefg";
    echo "{".$state."}";//返回状态值，加“{”是为了帮助Arduino确定数据的位置
}else{
	echo "Permission Failed";//请求中没有type或data或token或token错误时，显示Permission Denied
}
?>
