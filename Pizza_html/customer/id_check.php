<?
	include 'oracle_connect.php'
?>
<html>

<head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<title>아이디 체크</title>
</head>

<body bgcolor='white' text='black' link='blue' vlink='purple' alink='red'>
<table border='1' width='300' align='center' cellspacing='1' bordercolordark='white' bordercolorlight='black'>
<?

	if($p_id) {

		$query="BEGIN p_id_find('$p_id',:result); END;";
		$stmt=OCIParse($conn,$query); 
		OCIBindByName ($stmt,":result",&$result,30); 
		OCIExecute($stmt); 
		OCILogOff($conn);

		if($result=="TRUE") {
			echo("
	<tr>
		<td width='300' align='center'>
			<p>$p_id</p>
		</td>
	</tr>	
	<tr>
		<td width='300' align='center'>
			<p>등록된 아이디입니다.</p>
		</td>
	</tr>
			");
		}
		else {
			echo("
	<tr>
		<td width='300' align='center'>
			<p>$p_id</p>
		</td>
	</tr>
	<tr>
		<td width='300' align='center'>
			<p>사용가능한 아이디입니다.</p>
		</td>
	</tr>
			");
		}
	}
	else {
		echo("
	<tr>
		<td width='300' align='center'>
			<p>아이디를 입력해 주십시오.</p>
		</td>
	</tr>
		");
	}
?>
	<tr>
		<td width='300' align='center'>
			<p><a href='#' onClick='self.close()'>닫기</a></p>
		</td>
	</tr>
</table>

</body>
</html>
