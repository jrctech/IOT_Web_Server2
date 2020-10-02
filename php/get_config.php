<?php
    require_once("conectar.php");
    $conexion = conectar();
    $config_values = mysqli_query($conexion, "SELECT `AC`, `A_AC`, `LA`, `A_LA`, `LB`, `A_LB`, `LC`, `A_LC`, `V`, `TR`, `AR`, `BR`, `CR`, `PW` FROM `config_values` WHERE `id`=1");
    $config_values=mysqli_fetch_array($config_values);
    
    echo "" . $config_values['AC'] . ","
            . $config_values['A_AC'] . ","
            . $config_values['LA'] . ","
            . $config_values['A_LA'] . ","
            . $config_values['LB'] . ","
            . $config_values['A_LB'] . ","
            . $config_values['LC'] . ","
            . $config_values['A_LC'] . ","
            . $config_values['V'] . ","
            . $config_values['TR'] . ","
            . $config_values['AR'] . ","
            . $config_values['BR'] . ","
            . $config_values['CR'] . ","
            . $config_values['PW'];
    mysqli_close($conexion);

?>