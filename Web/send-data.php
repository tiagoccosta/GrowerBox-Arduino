<?php 

header("Content-Type: application/json; charset=UTF-8");
include('data/credentials.php');
$sucess = false;
$error = "";

$checked = false;
if(isset($_REQUEST['user']) && isset($_REQUEST['pass'])){
	if($_REQUEST['user'] == $user && $_REQUEST['pass'] == $pass){
		$checked=true;
	}else{
		$error = 'Usuario e/ou password não conferem.';
	}
}else{
	$error = 'Não foram inseridos dados de permissão.';
}

//$checked = true;
if($checked == true){
	try{
		$content = file_get_contents('php://input');
		//echo($content);
		$jsonString = $content;
		$fp = fopen('data/data.json', 'w');
		fwrite($fp, $jsonString);
		fclose($fp);
		$sucess = true;
	} catch(Exception $e){ $error = 'Caught exception: '.$e->getMessage();}
}



$retorno = array(
	'Success' => $sucess,
	'Error' => $error
);
echo(json_encode($retorno));
?>