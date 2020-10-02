	var ajaxActualizar=null;
	var x=null;
	var ping = null;
	

	$(document).ready(function(){
		if(document.getElementById('rngTref') != null){
		//testPing();
		document.querySelector('#rngTref').addEventListener('input', function() {document.getElementById('lblTref').innerHTML = 'Temperatura de referencia: ' + document.getElementById('rngTref').value + '\u00B0C';}, false);
		document.querySelector('#rngRefLuzA').addEventListener('input', function() {document.getElementById('lblRefLuzA').innerHTML = 'Nivel de referencia: ' + document.getElementById('rngRefLuzA').value + '%';}, false);
		document.querySelector('#rngRefLuzB').addEventListener('input', function() {document.getElementById('lblRefLuzB').innerHTML = 'Nivel de referencia: ' + document.getElementById('rngRefLuzB').value + '%';}, false);
		document.querySelector('#rngRefLuzC').addEventListener('input', function() {document.getElementById('lblRefLuzC').innerHTML = 'Nivel de referencia: ' + document.getElementById('rngRefLuzC').value + '%';}, false);
		document.querySelector('#rngVentPow').addEventListener('input', function() {document.getElementById('lblPow').innerHTML = 'Potencia: ' + document.getElementById('rngVentPow').value + '%';}, false);
		//document.getElementById('btnBorrarEEPROM').disabled='false';
		}
	});

	document.addEventListener("DOMContentLoaded", function(){if(document.getElementById('rngTref') != null) testPing();});
	

	function testPing(){
		
		//var urlPing="http://iotwebserver.ddns.net/token=00001&ping";
		var urlPing=getAbsolutePath() + 'ping';
		ping = new Date;
		$.ajax({ type: "GET",
			url: urlPing,
			cache:false,
			success: function(output){ 
				ping = new Date - ping;
				if(ping<1800)  //1000
					timeActualizar=1500;  //2500
				else if(ping<2800) //2000
					timeActualizar=3000;  //5000
				else if(ping<5300)  //4500
					timeActualizar=5500; //10000
				else
					timeActualizar=6500; //15000
				document.getElementById('alertPing').innerHTML='<strong>Ping: </strong>'+ping+' ms. <strong>Tiempo Actualizaci&oacute;n: </strong>'+timeActualizar+ ' ms.';
				x=setInterval('actualizar()',timeActualizar);
				
				var lista = (output.split(','));
				for (var i=0; i<=8; i++){
					if(lista[i]=='1')
						lista[i]=true;
					else
						lista[i]=false;
				}
				
				document.getElementById('chkAC').checked=lista[0];
				document.getElementById('chkACauto').checked=lista[1];
				document.getElementById('chkLuzA').checked=lista[2];
				document.getElementById('chkLuzAutoA').checked=lista[3];
				document.getElementById('chkLuzB').checked=lista[4];
				document.getElementById('chkLuzAutoB').checked=lista[5];
				document.getElementById('chkLuzC').checked=lista[6];
				document.getElementById('chkLuzAutoC').checked=lista[7];
				document.getElementById('chkVent').checked=lista[8];
				
				autoTemp();
				autoLuz('A');
				autoLuz('B');
				autoLuz('C');
				
				document.getElementById('rngTref').value=lista[9];
				document.getElementById('lblTref').innerHTML='Temperatura de referencia: '+ lista[9] + '\u00B0C';
				document.getElementById('rngRefLuzA').value=lista[10];
				document.getElementById('lblRefLuzA').innerHTML='Nivel de referencia: '+ lista[10] + '%';
				document.getElementById('rngRefLuzB').value=lista[11];
				document.getElementById('lblRefLuzB').innerHTML='Nivel de referencia: '+ lista[11] + '%';
				document.getElementById('rngRefLuzC').value=lista[12];
				document.getElementById('lblRefLuzC').innerHTML='Nivel de referencia: '+ lista[12] + '%';
				document.getElementById('rngVentPow').value=lista[13];
				document.getElementById('lblPow').innerHTML='Potencia: '+ lista[13] + '%';
				
				
				
				$.ajaxSetup({timeout: timeActualizar, error: function(){}});
			},
			error: function(){setTimeout(testPing(),2000);}
		});
	}
	
	function autoTemp(){
		if (document.getElementById('chkACauto').checked){
			document.getElementById('chkAC').disabled=true;
			document.getElementById('rngTref').disabled=false;
		}
		else {
			document.getElementById('chkAC').disabled=false;
			document.getElementById('rngTref').disabled=true;
		}
	}

	function autoLuz(luz){
		if (document.getElementById('chkLuzAuto'+luz).checked){
			document.getElementById('chkLuz'+luz).disabled=true;
			document.getElementById('rngRefLuz'+luz).disabled=false;
		}
		else {
			document.getElementById('chkLuz'+luz).disabled=false;
			document.getElementById('rngRefLuz'+luz).disabled=true;
		}
	}
	
	function trefChange(){
		document.getElementById('lblTref').innerHTML='Temperatura de referencia: '+ document.getElementById('rngTref').value + '\u00B0C';
	}
	
	function refAChange(){
		document.getElementById('lblRefLuzA').innerHTML='Nivel de referencia: '+ document.getElementById('rngRefLuzA').value + '%';
	}
	function refBChange(){
		document.getElementById('lblRefLuzB').innerHTML='Nivel de referencia: '+ document.getElementById('rngRefLuzB').value + '%';
	}
	function refCChange(){
		document.getElementById('lblRefLuzC').innerHTML='Nivel de referencia: '+ document.getElementById('rngRefLuzC').value + '%';
	}
	function powChange(){
		document.getElementById('lblPow').innerHTML='Potencia: '+ document.getElementById('rngVentPow').value + '%';
	}


	function actualizar(){
		if(((ajaxActualizar == null) || (ajaxActualizar.readyState==4) || (ajaxActualizar.readyState==0)) ){
			url=getAbsolutePath() + 'actBits';
			//url="http://iotwebserver.ddns.net/token=00001&actBits";
						
			var dateTime=new Date();
			var day=dateTime.getDate();
			var month=dateTime.getMonth() + 1;
			var yr=dateTime.getFullYear() - 2000;
			var dow=dateTime.getDay();
			var hrs=dateTime.getHours();
			var min=dateTime.getMinutes();
			var sec=dateTime.getSeconds();			
						
			ajaxActualizar=$.post(url, 
			{AC: chkAC.checked,
			 A_AC: chkACauto.checked,
			 LA: chkLuzA.checked,
			 A_LA: chkLuzAutoA.checked,
			 LB: chkLuzB.checked,
			 A_LB: chkLuzAutoB.checked,
			 LC: chkLuzC.checked,
			 A_LC: chkLuzAutoC.checked,
			 V: chkVent.checked,
			 TR: rngTref.value,
			 AR: rngRefLuzA.value,
			 BR: rngRefLuzB.value,
			 CR: rngRefLuzC.value,
			 PW: rngVentPow.value,
			 token: document.getElementById('token').value,
			 fin:null
			 }, function(data){mostrar(data);});
		}
	}
	
	function mostrar(respuesta){
		if(respuesta!='Token no valido'){
			var valores = (respuesta.split(','));
			document.getElementById('Tact').innerHTML=valores[0] + '\u00B0C';
			document.getElementById('LDR_1').innerHTML=valores[1] + '%';
			document.getElementById('LDR_2').innerHTML=valores[2] + '%';
			document.getElementById('LDR_3').innerHTML=valores[3] + '%';
			document.getElementById('V9V').innerHTML=valores[4] + ',' + valores[5] + 'V';
			document.getElementById('V5V').innerHTML=valores[6] + ',' + valores[7] + 'V';
			document.getElementById('V3V3').innerHTML=valores[8] + ',' + valores[9] + 'V';
			if(parseInt(valores[11],10) < 10)
				valores[11]='0'+valores[11];
			if(parseInt(valores[12],10) < 10)
				valores[12]='0'+valores[12];
			if(parseInt(valores[13],10) < 10)
				valores[13]='0'+valores[13];
			if(parseInt(valores[14],10) < 10)
				valores[14]='0'+valores[14];
			if(parseInt(valores[15],10) < 10)
				valores[15]='0'+valores[15];
			if(parseInt(valores[16],10) < 10)
				valores[16]='0'+valores[16];
			document.getElementById('alertClock').innerHTML='<p align="center"><strong>' + valores[10] + '&nbsp;' + valores[11] + '/' + valores[12] + '/20' + valores[13] + '&nbsp;&nbsp;&nbsp;&nbsp;' + valores[14] + ':' + valores[15] + ':' + valores[16] + '</strong></a>';
			
		}
		else{
			clearInterval(x);
			alert('Token no v\u00E1lido');
		}
		
		
	}
	
	function actualizarRTC(){
		if (((ajaxActualizar == null) || (ajaxActualizar.readyState==4) || (ajaxActualizar.readyState==0)) ){
			var r=confirm("\u00BFDesea actualizar el reloj del sistema con la hora y fecha de este equipo?");
			if (r==true){
				var dateTime=new Date();
				if(dateTime.getDate()<10){
					var day='0' + dateTime.getDate();
				}
				else
					var day=dateTime.getDate();

				if((dateTime.getMonth() + 1) <10){
					var month='0' + (dateTime.getMonth()+1);
				}
				else
					var month=dateTime.getMonth() + 1;
				
				var yr=dateTime.getFullYear() - 2000;
				
				var dow=dateTime.getDay();
				
				if(dateTime.getHours()<10){
					var hrs='0' + dateTime.getHours();
				}
				else
					var hrs=dateTime.getHours();
				
				if(dateTime.getMinutes()<10){
					var min='0' + dateTime.getMinutes();
				}
				else
					var min=dateTime.getMinutes();
				
				if(dateTime.getSeconds()<10){
					var sec='0' + dateTime.getSeconds();
				}
				else
					var sec=dateTime.getSeconds();
					
				url=getAbsolutePath()+'actRTC';
				//url="http://iotwebserver.ddns.net/token=00001&actRTC";
							
				ajaxActRTC=$.post(url, 
				{
					Z:null,
					A: day,
					B: month,
					C: yr,
					D: dow,
					E: hrs,
					F: min,
					G: sec,
					token: document.getElementById('token').value,
					fin: 123451234567  //Caracteres extra para completar la linea
				 }, function(data){alert(data); if(document.getElementById('alertInfo') != null) window.location.href=getAbsolutePath()+'viewlog?token='+document.getElementById('token').value;});
			}
		}
	}
	function btnBorrarClick(){
		var r=confirm("\u00BFDesea eliminar todos los registros?");
		if (r==true){
			url=getAbsolutePath()+'eraseMem';
			//url="http://iotwebserver.ddns.net/token=00001&aeraseMem";
			ajaxEraseMem=$.get(url, {token: document.getElementById('token').value}, 
				function(data){alert(data);
				window.location.href=getAbsolutePath()+'viewlog?token='+document.getElementById('token').value;
				});
			
		}
	}
	
	function btnPrevClick(){
		window.location.href=getAbsolutePath()+'prev?' + 'token='+document.getElementById('token').value;
	}
	
	function btnNextClick(){
		window.location.href=getAbsolutePath()+'next?' + 'token='+document.getElementById('token').value;
	}
	
	
	
	/* function getAbsolutePath() {
		var loc = window.location;
		console.log(loc);
		var pathName = loc.pathname.substring(0, loc.pathname.lastIndexOf('/') + 1);
		console.log(pathName);
		var resultado = loc.href.substring(0, loc.href.length - ((loc.pathname + loc.search + loc.hash).length - pathName.length));
		return resultado;
	} */
	
	function getAbsolutePath() {
		var loc = window.location;
		var resultado = loc.origin + '/';
		return resultado;
	}

	function cerrarSesion(){
		var r=confirm("\u00BFDesea cerrar sesi\u00F3n en JRC - IoT Web Server?");
		if (r==true){
			window.location.href=getAbsolutePath()+'logout?' + 'token='+document.getElementById('token').value;
			//window.location.href='http://iotwebserver.ddns.net/&token=00001&logout';
		}
	}
	
	function homeClick(){
		window.location.href=getAbsolutePath()+'home?' + 'token='+document.getElementById('token').value;
	}
	
	function visorClick(){
		window.location.href=getAbsolutePath()+'viewlog?' + 'token='+document.getElementById('token').value;
		
	}