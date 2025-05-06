<?php
require '../vendor/autoload.php';

function get_db() {
    $client = new MongoDB\Client(
        "mongodb://localhost:27017",
        [
            'username' => 'wai_web',    
            'password' => 'w@i_w3b',    
            'authSource' => 'wai'
        ]
    );
    $db = $client->wai;
    return $db;
}
?>
