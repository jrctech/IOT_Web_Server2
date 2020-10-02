<?php 
/*     $tblEvent =   array("Encendido",
                        "Inicio Sesión",
                        "Actualizar Reloj",
                        "Cierre Sesión",
                        "AC",
                        "A_AC",
                        "LA",
                        "A_LA",
                        "LB",
                        "A_LB",
                        "LC",
                        "A_LC",
                        "V",
                        "TR",
                        "AR",
                        "BR",
                        "CR",
                        "PW"); */

    $date_time = $_POST['date_time'];
    $event = $_POST['event'];
    $value = $_POST['value'];
    $description = $_POST['description'];
    require_once('conectar.php');
    $conexion=conectar();
    mysqli_query($conexion, "INSERT INTO `event_log`(`id`, `date_time`, `event`, `description`, `value`) VALUES (NULL,'$date_time',$event,'$description',$value)");
    echo "$description $event $value $date_time Success!";

    $cuenta=mysqli_query($conexion,"SELECT COUNT(*) FROM `event_log` WHERE 1");
    $cuenta=mysqli_fetch_array($cuenta);
    if ($cuenta[0] >= 1000){
        mysqli_query($conexion, "DELETE FROM event_log ORDER BY id LIMIT 1;");
    }

    switch ($event)
    {
        case 4:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`AC`=$value WHERE 1");
        break;
        case 5:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`A_AC`=$value WHERE 1");
        break;
        case 6:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`LA`=$value WHERE 1");
        break;
        case 7:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`A_LA`=$value WHERE 1");
        break;
        case 8:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`LB`=$value WHERE 1");
        break;
        case 9:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`A_LB`=$value WHERE 1");
        break;
        case 10:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`LC`=$value WHERE 1");
        break;
        case 11:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`A_LC`=$value WHERE 1");
        break;
        case 12:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`V`=$value WHERE 1");
        break;
        case 13:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`TR`=$value WHERE 1");
        break;
        case 14:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`AR`=$value WHERE 1");
        break;
        case 15:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`BR`=$value WHERE 1");
        break;
        case 16:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`CR`=$value WHERE 1");
        break;
        case 17:
            mysqli_query($conexion, "UPDATE `config_values` SET `id`=1,`PW`=$value WHERE 1");
        break;

    }
    mysqli_close($conexion);
    


?> 