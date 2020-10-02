

<?php
    require_once("conectar.php");
    $conexion = conectar();
    mysqli_query($conexion, "DELETE FROM `event_log` WHERE 1");
    mysqli_query($conexion,"ALTER TABLE event_log DROP id");
    mysqli_query($conexion,"ALTER TABLE event_log AUTO_INCREMENT = 1");
    mysqli_query($conexion,"ALTER TABLE event_log ADD id bigint UNSIGNED NOT NULL AUTO_INCREMENT PRIMARY KEY FIRST");
    echo "Events deleted!";
    mysqli_close($conexion);
?>