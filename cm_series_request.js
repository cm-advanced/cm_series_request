(function ($) {
  Drupal.behaviors.seriesRequest = {
    attach: function (context, settings) { 
      ////////////////////////////////////////////////////////////////////////
      //ON READY FUNCTION
      $(document).ready(function() {
	for (var i =0; i<= 5; i++) {
	  
          var var_prefix = 'edit-field-series-request-admin-und-' + i;
          var start_time_id =  var_prefix + '-field-start-time-und';
          var start_time = $('#' + start_time_id).val();

	  cmSeriesRequestUpdateScheduleFields(i, start_time);
	}
      });
      ////////////////////////////////////////////////////////////////////////
      //CHANGE FUNCTION FOR LIVESOURCE DROPDOWN
      $("select[id$='field-live-source-und']", context).change(function() {
	var row_index = $(this).attr('id').substring(36,37);

	var var_prefix = 'edit-field-series-request-admin-und-' + row_index;
	var start_time_id =  var_prefix + '-field-start-time-und';
	$('#' + start_time_id).val(0);

	cmSeriesRequestUpdateScheduleFields(row_index, 0);
      });
      ////////////////////////////////////////////////////////////////////////
      //CHANGE FUNCTION FOR CHANNEL DROPDOWN
      $("select[id$='field-airing-channel-und']", context).change(function() {
	var row_index = $(this).attr('id').substring(36,37);

	var var_prefix = 'edit-field-series-request-admin-und-' + row_index;
	var start_time_id =  var_prefix + '-field-start-time-und';
	$('#' + start_time_id).val(0);

	$('#' + start_time_id).val(""); 
	cmSeriesRequestUpdateScheduleFields(row_index, 0);
      });
      ////////////////////////////////////////////////////////////////////////
      //CHANGE FUNCTION FOR START DAY DROPDOWN
      $("select[id$='field-start-day-und']", context).change(function() {
	var row_index = $(this).attr('id').substring(36,37);

	var var_prefix = 'edit-field-series-request-admin-und-' + row_index;
	var start_time_id =  var_prefix + '-field-start-time-und';
	$('#' + start_time_id).val(0);

	cmSeriesRequestUpdateScheduleFields(row_index, 0);
      });
      ////////////////////////////////////////////////////////////////////////
      //CHANGE FUNCTION FOR START TIME DROPDOWN
      $("select[id$='field-start-time-und']", context).change(function() {
	var row_index = $(this).attr('id').substring(36,37);

	var var_prefix = 'edit-field-series-request-admin-und-' + row_index;
	var start_time_id =  var_prefix + '-field-start-time-und';
	var start_time = $('#' + start_time_id).val();

	cmSeriesRequestCheckScheduledFields(row_index, start_time);
      });            
      ////////////////////////////////////////////////////////////////////////
      //function is passed a row index and will decide how to display the
      //fields for scheduling information (used to auto create airings)
      function cmSeriesRequestUpdateScheduleFields(row_index, start_time) {
	var var_prefix = 'edit-field-series-request-admin-und-' + row_index;
	var channel_id =  var_prefix + '-field-airing-channel-und';
	var channel = $('#' + channel_id).val(); 
	var start_day_id =  var_prefix + '-field-start-day-und';
	var start_day = $('#' + start_day_id).val();
	var start_time_id =  var_prefix + '-field-start-time-und';

	var live_source_id =  var_prefix + '-field-live-source-und';
	var live_source = $('#' + live_source_id).val(); 
	if (!channel) {
	  return;
	}
	var runtime = $('#edit-field-runtime-default-und').val();
        
	var item = new Date();
	console.log(item.toTimeString());
	console.log(start_time);
	var none_value = "_none";

	//IF ITS NOT THE PREMIERE ROW, DISABLE SOURCE
	if (row_index != 0) {
	 $('#' + live_source_id).attr("disabled", "disabled");
	}
	//HIDE DAY AND TIME IF WE DON'T HAVE CHANNEL
	if (channel == none_value) {
	  $('#' + start_day_id).val(none_value);
	  $('#' + start_time_id).val(none_value);
	  $('#' + start_day_id).parent().parent().hide();
	  $('#' + start_time_id).parent().parent().hide();
	}
	//HIDE TIME IF WE DONT HAVE THE OTHER 3 FIELDS
	else if (channel == none_value || start_day == none_value) {
	  $('#' + start_day_id).parent().parent().show();
	  $('#' + start_time_id).parent().parent().hide();
	  $('#' + start_time_id).val(none_value);
	}
	else {
	  $('#' + start_day_id).parent().parent().show();
	  $('#' + start_time_id + ' option:gt(0)').remove();
	  $('#' + start_time_id + ' option:eq(0)').remove();
	  $('#' + start_time_id).parent().parent().show();
	  
	  
	         
	  var override = getURLParameter('override');
	  if (override) {
	    override = "?override=true";
	  }
	  else {
	    override = "";
	  }
	  console.log(override);
	  console.log('override');
	  

	  var cm_agd_url = '/admin/programming/series_request/ajax_times/' + 
	    live_source + '/' + channel + '/' + start_day + '/' + start_time +
            '/' + runtime + override;
          $.getJSON(cm_agd_url, function(data){
	    $.each(data, function(i,item){
	      $('#' + start_time_id).append($("<option></option")
					    .attr("value", item.time_id)
					    .text(item.time_id));
	      $('#' + start_time_id).val(start_time);
	    });
          });           
	}
      }

              ////////////////////////////////////////////////////////////////////////
      //function is passed a row index and will decide how to display the
      //fields for scheduling information (used to auto create airings)
      function cmSeriesRequestCheckScheduledFields(row_index, start_time) {
	var var_prefix = 'edit-field-series-request-admin-und-' + row_index;
	var channel_id =  var_prefix + '-field-airing-channel-und';
	var channel = $('#' + channel_id).val(); 
	var start_day_id =  var_prefix + '-field-start-day-und';
	var start_day = $('#' + start_day_id).val();
	var start_time_id =  var_prefix + '-field-start-time-und';

	var live_source_id =  var_prefix + '-field-live-source-und';
	var live_source = $('#' + live_source_id).val(); 
	if (!channel) {
	  return;
	}
	
	         
	var override = getURLParameter('override');
	if (override) {
	  override = "?override=true";
	}
	else {
	  override = "";
	}
	console.log(override);
	console.log('override');


	var runtime = $('#edit-field-runtime-default-und').val();
        
	var item = new Date();
	console.log(item.toTimeString());
	console.log(start_time);
	var none_value = "_none";
	  
        var cm_agd_url = '/admin/programming/series_request/ajax_check_scheduled_combo/' + 
          live_source + '/' + channel + '/' + start_day + '/' + start_time +
          '/' + runtime + override;
  // not setting but just checking to see if valid - perhpas don't need a return at all
        $.getJSON(cm_agd_url, function(data){
          $.each(data, function(i,item){
            $('#' + start_time_id).append($("<option></option")
                                          .attr("value", item.time_id)
                                          .text(item.time_id));
            $('#' + start_time_id).val(start_time);
          });
        });         
      }

      function getURLParameter(sParam)                                          
      {                                
        var sPageURL = window.location.search.substring(1);
        var sURLVariables = sPageURL.split('&');
        for (var i = 0; i < sURLVariables.length; i++)
        {                                                                      
          var sParameterName = sURLVariables[i].split('='); 
          if (sParameterName[0] == sParam)
          {
            return sParameterName[1];
          }
        }
      }
      /*
            $('.cmag', context).click(function() {
                $('#airing_detail_div').html("<h2>Loading...</h2>");

                $('.cm_airing_grid_blank').css({"border":"none"});
                $(this).parent().parent().parent().parent().parent().
                    css({"border":"8px solid red"});

                $('html, body').animate({scrollTop:$('#airing_detail_div').
                                         offset().top - 75}, 'slow');
                $('div.contextual-links-wrapper').hide();            });
*/
    }};  
}) (jQuery);

