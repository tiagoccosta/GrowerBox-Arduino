<?php
$retorno = "";
 if (($_FILES["file"]["type"] == "image/png") && ($_FILES["file"]["size"] < 20000000000)) { 
 	if ($_FILES["file"]["error"] > 0){ 
    	$retorno .="\n ". "Return Code: " . $_FILES["file"]["error"] . ""; 
    } else { 
        //$retorno .= "\n ". "Upload: " . $_FILES["file"]["name"] . ""; 
        //$retorno .= "\n ". "Type: " . $_FILES["file"]["type"] . "";
        //$retorno .= "\n ". "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb";
        //$retorno .= "\n ". "Temp file: " . $_FILES["file"]["tmp_name"] . "";
		move_uploaded_file($_FILES["file"]["tmp_name"], "" . $_FILES["file"]["name"]);
		$retorno = "1";
	}
 }else {
	 $retorno .="\n ". "Invalid file";
 } 
echo $retorno;
?>
