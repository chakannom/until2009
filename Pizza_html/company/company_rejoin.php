<?
	session_start();

	if(!$com_id && !$com_pass) {
		echo("<meta http-equiv='refresh' content='0; url=login.php'>");
	}
	else {
		include 'oracle_connect.php';

		$query="SELECT c_id,c_name,c_tel,c_addr FROM company WHERE c_id='$com_id' and c_password='$com_pass'";
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
	if(f.c_password.value.length==0||f.c_repassword.value.length==0) {
		alert('패스워드를 입력해주세요.');
		f.c_password.focus();
		return (false);
	}
	if(f.c_name.value.length==0) {
		alert('이름을 입력해주세요.');
		f.c_name.focus();
		return (false);
	}
	if(f.c_tel.value.length==0) {
		alert('전화번호를 입력해주세요.');
		f.c_tel.focus();
		return (false);
	}
	if(f.c_addr.value.length == 0) {
		alert('주소를 입력해주세요.');
		f.c_addr.focus();
		return (false);
	}
	return true;		
}

function pass() {
	if(document.company_rejoin.c_password.value != document.company_rejoin.c_repassword.value) {
		alert('비밀번호가 일치하지 않습니다.');
		document.company_rejoin.c_password.value='';
		document.company_rejoin.c_repassword.value='';
		document.company_rejoin.c_password.focus();
		return(false);
	}
}

function before() {
	history.go(-1);
}

</script>
</head>

<body bgcolor='white' text='black' link='blue' vlink='purple' alink='red'>
<form name='company_rejoin' action='company_input.php' method='post' onSubmit='return test(this)'>
<table width='100%' height='100%' cellpadding='0' cellspacing='0'>
	<tr>
		<td align='center' valign='middle'>
<table border='1' width='543' cellspacing='0' bordercolordark='white' bordercolorlight='#75bbd4'>
	<tr>
		<td width='533' align='center' colspan='2'>
			<font size='6' face='HY나무M'><b>기업 정보 수정</b></font>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			아이디
		</td>
		<td width='377' align='left'>
			<? echo $row[0]; $c_id=$row[0]; ?>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			패스워드
		</td>
		<td width='377' align='left'>
			<input type='password' name='c_password' maxlength='10' size='12'>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			패스워드 확인
		</td>
		<td width='377' align='left'>
			<input type='password' name='c_repassword' maxlength='10' size='12'>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			기업명
		</td>
		<td width='377' align='left'>
			<input type='text' name='c_name' maxlength='20' size='12' value='<? echo $row[1]; ?>'>
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			연락처
		</td>
		<td width='377' align='left'>
			<input type='text' name='c_tel' maxlength='16' size='17' value='<? echo $row[2]; ?>'> (예: 000-000-0000)
		</td>
	</tr>
	<tr>
		<td width='150' align='center' bgcolor='#EEF0F4'>
			주&nbsp;&nbsp;소
		</td>
		<td width='377' align='left'>
			<input type='text' name='c_addr' maxlength='128' size='50' value='<? echo $row[3]; ?>'>
		</td>
	</tr>
	<tr>
		<td width='533' align='center' colspan='2'>
				<input type='submit' name='input' value='   확 인   ' onClick='pass()'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type='button' name='BT_before' value='전단계로' onClick='before()'>
			
		</td>
	</tr>
</table>
		</td>
	</tr>
</table>
</form>

</body>
</html>
