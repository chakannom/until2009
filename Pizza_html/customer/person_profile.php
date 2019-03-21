<?
	session_start();

	if(!$login_id && !$login_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
	else {
		include 'oracle_connect.php';

		$query="SELECT p_id,p_name,p_tel,p_addr FROM person WHERE p_id='$login_id' and p_password='$login_pass'";
		$stmt=OCIParse($conn,$query);
		OCIExecute($stmt);
		OCIFetch($stmt);
		$row=array(OCIResult($stmt,1),OCIResult($stmt,2),OCIResult($stmt,3),OCIResult($stmt,4));
		OCIFreeStateMent($stmt);
		OCILogOff($conn);
	}
?>

<html>

<head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<title>정보 확인</title>
</head>

<body bgcolor='white' text='black' link='blue' vlink='purple' alink='red'>
<table width='980' cellpadding='0' cellspacing='0'>
    <tr>
        <td width='980' align='center'>
<table border='1' width='543' cellspacing='0' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='533' align='center' colspan='2'>
			<p><font size='6'><b>정보 확인</b></font></p>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			<p>아이디</p>
		</td>
		<td width='377' align='left'>
			<p><? echo $row[0]; ?></a></p>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			<p>이 &nbsp;&nbsp;름</p>
		</td>
		<td width='377' align='left'>
			<p><? echo $row[1]; ?></p>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			<p>연락처</p>
		</td>
		<td width='377' align='left'>
			<p><? echo $row[2]; ?></p>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			<p>주 &nbsp;&nbsp;소</p>
		</td>
		<td width='377' align='left'>
			<p><? echo $row[3]; ?></p>
		</td>
	</tr>
	<tr>
		<td width='533' align='center' colspan='2'>
				<p><a href='person_secession.php'>탈&nbsp;&nbsp;퇴</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='person_rejoin.php'>수&nbsp;&nbsp;정</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='../main.html'>메인으로</a></p>
		</td>
	</tr>
</table>
		</td>
	</tr>
</table>
</form>

</body>
</html>
