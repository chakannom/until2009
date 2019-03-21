<?
	session_start();

	if(!$com_id && !$com_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
	else {
		include 'oracle_connect.php';

		$query="SELECT c_id,c_name,c_tel,c_addr,c_ok FROM company WHERE c_id='$com_id' and c_password='$com_pass'";
		$stmt=OCIParse($conn,$query);
		OCIExecute($stmt);
		OCIFetch($stmt);
		$row=array(OCIResult($stmt,1),OCIResult($stmt,2),OCIResult($stmt,3),OCIResult($stmt,4),OCIResult($stmt,5));
		OCIFreeStateMent($stmt);
		OCILogOff($conn);
	}
?>

<html>

<head>
<meta http-equiv='content-type' content='text/html; charset=utf-8'>
<title>기업 정보 확인</title>
</head>

<body bgcolor='white' text='black' link='blue' vlink='purple' alink='red'>
<table width='100%' height='100%' cellpadding='0' cellspacing='0'>
    <tr>
        <td align='center' valign='middle'>
<table border='1' width='543' cellspacing='0' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='533' align='center' colspan='2'>
			<font size='6' face='HY나무M'><b>기업 정보 확인</b></font>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			아이디
		</td>
		<td width='377' align='left'>
			<? echo $row[0]; ?>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			이&nbsp;&nbsp;름
		</td>
		<td width='377' align='left'>
			<? echo $row[1]; ?>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			연락처
		</td>
		<td width='377' align='left'>
			<? echo $row[2]; ?>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			주&nbsp;&nbsp;소
		</td>
		<td width='377' align='left'>
			<? echo $row[3]; ?>
		</td>
	</tr>
	<tr>
		<td width='533' align='center' colspan='2'>
				<a href='company_secession.php'>탈&nbsp;&nbsp;퇴</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='company_rejoin.php'>수&nbsp;&nbsp;정</a>
		</td>
	</tr>
</table>
		</td>
	</tr>
</table>
</form>

</body>
</html>
