(function() {

	var loadConfig = function () {
		console.log(localStorage);
		if(localStorage.type) {
			$('input[name=displayType][value="' + localStorage.type + '"]').prop('checked',true);
		}
		if(localStorage.model) {
			$('input[name=jeepModel][value="' + localStorage.model + '"]').prop('checked', true);
		}
		if(localStorage.seconds) {
			$('#showSeconds').prop('checked',(localStorage.seconds === "true"));
		}
		if(localStorage.twentyFour) {
			$('#twentyFour').prop('checked',(localStorage.twentyFour === "true"));
		}
		if(localStorage.jeepColor) {
			$('#jeepColor')[0].value = localStorage.jeepColor;
		}
		if(localStorage.timeColor) {
			$('#timeColor')[0].value = localStorage.timeColor;
		}
		if(localStorage.bgColor) {
			$('#bgColor')[0].value = localStorage.bgColor;
		}

	}

	var saveConfig = function() {
		var $type = $('input[name=displayType]:checked');
		var $model = $('input[name=jeepModel]:checked');
		var $seconds = $('#showSeconds');
		var $twentyFour = $('#twentyFour');
		var $jeepColor = $('#jeepColor');
		var $timeColor = $('#timeColor');
		var $bgColor = $('#bgColor');

		var data = {
			type: parseInt($type.val()),
			model: parseInt($model.val()),
			seconds: $seconds[0].checked,
			twentyFour: $twentyFour[0].checked,
			jeepColor: $jeepColor.val(),
			timeColor: $timeColor.val(),
			bgColor: $bgColor.val(),
		}
 
		localStorage.type = data.type;
		localStorage.model = data.model;
		localStorage.seconds = data.seconds;
		localStorage.twentyFour = data.twentyFour;
		localStorage.jeepColor = data.jeepColor;
		localStorage.timeColor = data.timeColor;
		localStorage.bgColor = data.bgColor;

		console.log("New settings: " + JSON.stringify(data, null, 4));
		return data;
	}

	var getQueryParam = function() {
		var query = location.search.substring(1);
		var args = query.split('&');
		for (var i = args.length - 1; i >= 0; i--) {
			var pair = args[i].split('=');
			if(pair[0] === "return_to") {
				console.log("Return to: " + pair[1]);
				return decodeURIComponent(pair[1]);
			}
		}
		console.log('Return to: pebblejs://close#');
		return 'pebblejs://close#';
	}


	$('#saveButton').on('click', function() {
		document.location = getQueryParam() + encodeURIComponent(JSON.stringify(saveConfig()));
	});

	loadConfig();

})()




