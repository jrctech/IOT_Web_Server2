<?php
    require_once("conectar.php");
    $conexion = conectar();
    $report = mysqli_query($conexion, "SELECT `date_time` FROM `last_report` WHERE `id`=1");
    $report=mysqli_fetch_array($report);
    echo $report['date_time'];
    mysqli_close($conexion);
?>