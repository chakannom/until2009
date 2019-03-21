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
<title>회원 정보 수정</title>
<script>
function test(f) {
	if(f.p_password.value.length==0||f.p_repassword.value.length==0) {
		alert('패스워드를 입력해주세요.');
		f.p_password.focus();
		return (false);
	}
	if(f.p_name.value.length==0) {
		alert('이름을 입력해주세요.');
		f.p_name.focus();
		return (false);
	}
	if(f.p_tel.value.length==0) {
		alert('전화번호를 입력해주세요.');
		f.p_tel.focus();
		return (false);
	}
	if(f.p_addr.value.length == 0) {
		alert('주소를 입력해주세요.');
		f.p_addr.focus();
		return (false);
	}
	return true;		
}

function pass() {
	if(document.person_rejoin.p_password.value != document.person_rejoin.p_repassword.value) {
		alert('비밀번호가 일치하지 않습니다.');
		document.person_rejoin.p_password.value='';
		document.person_rejoin.p_repassword.value='';
		document.person_rejoin.p_password.focus();
		return(false);
	}
}
	
function idcheck() {
	window.open('id_check.php?p_id=' + document.person_rejoin.p_id.value,'idwin','width=350,height=100,toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=no,resizable=yes');
}

</script>
</head>

<body bgcolor='white' text='black' link='blue' vlink='purple' alink='red'>
<form name='person_rejoin' action='person_input.php' method='post' onSubmit='return test(this)'>
<table width='980' cellpadding='0' cellspacing='0'>
	<tr>
		<td width='980' align='center'>
<table border='1' width='543' cellspacing='0' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='533' align='center' colspan='2'>
			<p><font size='6'><b>회원 정보 수정</b></font></p>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			<p>아이디</p>
		</td>
		<td width='377' align='left'>
			<p><? echo $row[0]; $p_id=$row[0]; ?></p>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			<p>패스워드</p>
		</td>
		<td width='377' align='left'>
			<p><input type='password' name='p_password' maxlength='10' size='12'></p>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			<p>패스워드 확인</p>
		</td>
		<td width='377' align='left'>
			<p><input type='password' name='p_repassword' maxlength='10' size='12'></p>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			<p>이 &nbsp;&nbsp;름</p>
		</td>
		<td width='377' align='left'>
			<p><input type='text' name='p_name' maxlength='20' size='12' value='<? echo $row[1]; ?>'></p>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			<p>연락처</p>
		</td>
		<td width='377' align='left'>
			<p><input type='text' name='p_tel' maxlength='16' size='17' value='<? echo $row[2]; ?>'> (예: 000-000-0000)</p>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			<p>주 &nbsp;&nbsp;소</p>
		</td>
		<td width='377' align='left'>
			<p><input type='text' name='p_addr' maxlength='128' size='50' value='<? echo $row[3]; ?>'></p>
		</td>
	</tr>
	<tr>
		<td width='533' align='center' colspan='2'>
				<p><input type='submit' name='input' value='   확 인   ' onClick='pass()' > &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href='../main.html'>메인으로</a></p>
			
		</td>
	</tr>
</table>
		</td>
	</tr>
</table>
</form>

</body>
</html>
