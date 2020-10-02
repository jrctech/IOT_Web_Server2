<?php
    require_once("conectar.php");
    $conexion = conectar();
    $date_time = $_POST['date_time'];

    mysqli_query($conexion, "UPDATE `last_report` SET `id`=1,`date_time`='$date_time' WHERE 1");
    echo "Report received!";
    mysqli_close($conexion);
?>
