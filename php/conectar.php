<?php

function conectar()
{
    $conexion=mysqli_connect("localhost", "id7984111_javier310380", "Neuralgica310380.");
            mysqli_select_db($conexion, "id7984111_iotwebserver_db");
            mysqli_query($conexion, "SET NAMES 'utf8'");
            return $conexion;
}
?>