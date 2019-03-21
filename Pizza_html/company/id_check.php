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

	if($c_id) {

		$query="BEGIN c_id_find('$c_id',:result); END;";
		$stmt=OCIParse($conn,$query); 
		OCIBindByName ($stmt,":result",&$result,30); 
		OCIExecute($stmt); 
		OCILogOff($conn);

		if($result=="TRUE") {
			echo("
	<tr>
		<td width='300' align='center'>
			$c_id
		</td>
	</tr>	
	<tr>
		<td width='300' align='center'>
			등록된 아이디입니다.
		</td>
	</tr>
			");
		}
		else {
			echo("
	<tr>
		<td width='300' align='center'>
			$c_id
		</td>
	</tr>
	<tr>
		<td width='300' align='center'>
			사용가능한 아이디입니다.
		</td>
	</tr>
			");
		}
	}
	else {
		echo("
	<tr>
		<td width='300' align='center'>
			아이디를 입력해 주십시오.
		</td>
	</tr>
		");
	}
?>
	<tr>
		<td width='300' align='center'>
			<a href='#' onClick='self.close()'>닫기</a>
		</td>
	</tr>
</table>

</body>
</html>
