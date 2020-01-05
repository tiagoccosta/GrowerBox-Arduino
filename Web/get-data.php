<?php
   	
header("Content-Type: application/json; charset=UTF-8");
   $myFile = "data/data.json";
   $arr_data = array(); // create empty array

  try
  {
	  

	   //Get data from existing json file
	   $jsondata = file_get_contents($myFile);

	  echo $jsondata;	  

   }
   catch (Exception $e) {
            echo 'Caught exception: ',  $e->getMessage(), "\n";
   }

?>