<?php
error_reporting(E_ALL);

/* Get the port for the WWW service. */
$service_port = 33889;

/* Get the IP address for the target host. */
$address = "127.0.0.1";

/* Create a TCP/IP socket. */
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if ($socket === false) {
    // ERROR
    //echo "socket_create() failed: reason: " . 
          //socket_strerror(socket_last_error()) . "\n";
    echo 0;
    return;
}

// echo "Attempting to connect to '$address' on port '$service_port'...";
$result = socket_connect($socket, $address, $service_port);
if ($result === false) {
    //echo "socket_connect() failed.\nReason: ($result) " .
          //socket_strerror(socket_last_error($socket)) . "\n";
    echo 0;
    return;
}

$in = "operation:" . $_POST["operation"];
if($_POST["operation"] == "get-data"){
    $in = $in . " data:" . $_POST["data"];
}
foreach (array_keys($_POST) as $k=>$v){
    if($k == "operation" || $k == "data") continue;
    $in = $in . " " . $k . ":" . $v;
}
$out = '';

// echo "Sending HTTP HEAD request...";
socket_write($socket, $in, strlen($in));
// echo "OK.\n";

// echo "Reading response:\n\n";
while ($out = socket_read($socket, 2048)) {
    echo $out;
}

// echo "Closing socket...";
socket_close($socket);
// echo "OK.\n\n";

?>
