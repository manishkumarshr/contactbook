(function($)
{
	Drupal.behaviors.contactbook = {
		attach:function() {
			$("#edit-email").blur(function(){
				var email = $("#edit-email").val();
				var regex = /^([a-zA-Z0-9_\.\-\+])+\@(([a-zA-Z0-9\-])+\.)+([a-zA-Z0-9]{2,4})+$/;
				if(regex.test(email)) {
					$("#edit-email").css('color', 'green');
					$(".form-item-email .description").css('color', 'green');
					$(".form-item-email .description").html('Valid');
				} else {
					$("#edit-email").css('color', 'red');
					$(".form-item-email .description").css('color', 'red');
					$(".form-item-email .description").html('Invalid Email Id');
				}
			});
		}
	};
}(jQuery));