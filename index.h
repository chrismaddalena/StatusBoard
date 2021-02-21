const char INDEX_HTML[] PROGMEM = R"=====(
<html>
    <head>
        <meta charset='UTF-8'>
        <meta name='viewport' content='width=device-width, initial-scale=1.0'>
        <meta http-equiv='X-UA-Compatible' content='ie=edge'>
        <link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css' integrity='sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T' crossorigin='anonymous'>
        <script src='https://code.jquery.com/jquery-3.3.1.slim.min.js' integrity='sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo' crossorigin='anonymous'></script>
        <script src='https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js' integrity='sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1' crossorigin='anonymous'></script>
        <script src='https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js' integrity='sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM' crossorigin='anonymous'></script>
        <script src='https://code.jquery.com/jquery-3.5.1.min.js' integrity='sha256-9/aliU8dGd2tb6OSsuzixeV4y/faTqgFtohetphbbj0=' crossorigin='anonymous'></script>
        <link rel="preconnect" href="https://fonts.gstatic.com">
        <link href="https://fonts.googleapis.com/css2?family=Permanent+Marker&display=swap" rel="stylesheet">

        <title>Status Control Panel</title>

        <style>
          h1 {
           font-family: 'Permanent Marker'; 
          }
          .dark-mode {
            background: #343a40;
            color: #fff;
          }
        </style>
    </head>

    <body>
        <div id='alert-placeholder'></div>

        <h1 class='text-center'>Status Control Panel</h1>
        <table class='table table-borderless' style='margin-top: 20px'>
            <tr>
                <th class='align-middle'>In a Meeting</th>
                <td class='align-middle'><button id='btn-meeting' class='btn btn-lg col-sm-4 js-toggle-meeting'>Toggle</button></td>
            </tr>
            <tr>
                <th class='align-middle'>Do Not Disturb</th>
                <td class='align-middle'><button id='btn-dnd' class='btn btn-lg col-sm-4 js-toggle-dnd'>Toggle</button></td>
            </tr>
            <tr>
                <th class='align-middle'>Headphones</th>
                <td class='align-middle'><button id='btn-headphones' class='btn btn-lg col-sm-4 js-toggle-headphones'>Toggle</button></td>
            </tr>
            <tr>
                <th class='align-middle'>Needs Coffee</th>
                <td class='align-middle'><button id='btn-coffee' class='btn btn-lg col-sm-4 js-toggle-coffee'>Toggle</button></td>
            </tr>
            <tr>
                <th class='align-middle'>Reset LEDs</th>
                <td class='align-middle'><button id='btn-reset' class='btn btn-lg btn-warning col-sm-4 js-reset-led'>Reset</button></td>
            </tr>
        </table>

        <div class='align-middle text-center mt-5'>
          <button id='btn-mode-switch' class='btn btn-secondary col-sm-2 js-reset-led'>Swap Themes</button>
        </div>

        <script>
            function setBtnClasses(ele, value) {
                if (value == false) {
                    ele.removeClass('btn-success')
                    ele.addClass('btn-danger')
                    ele.html('Off');
                } else {
                    ele.removeClass('btn-danger')
                    ele.addClass('btn-success')
                    ele.html('On');
                }
            }

            function showAlert(message, alerttype) {
                $('#alert-placeholder').append('<div id=\"alertdiv\" class=\"alert ' +  alerttype + '\"><a class=\"close\" data-dismiss=\"alert\">×</a><span>' + message + '</span></div>')
                setTimeout(function() {
                    $("#alertdiv").remove();
                }, 2000);
            }

            $(document).ready(function() {
                var meeting_btn = $('#btn-meeting')
                var dnd_btn = $('#btn-dnd');
                var headphones_btn = $('#btn-headphones');
                var coffee_btn = $('#btn-coffee');

                function parseData(data) {
                    setBtnClasses(meeting_btn, data.meeting);
                    setBtnClasses(dnd_btn, data.dnd);
                    setBtnClasses(headphones_btn, data.headphones);
                    setBtnClasses(coffee_btn, data.coffee);
                };

                (function worker() {
                    $.ajax({
                        url: '/status',
                        success: function(data) {
                            parseData(data);
                        },
                        error: function( data, status, error ) { 
                            console.log(data);
                            console.log(status);
                            console.log(error);
                        },
                        complete: function() {
                            setTimeout(worker, 2500);
                        }
                    });
                })();

                $(function () {
                    $('#btn-meeting').on('click', function () {
                        $.ajax({
                            url: '/meeting',
                            success: function(data) {
                                if (data.result) {
                                    showAlert(data.message, 'alert-success')
                                } else {
                                    showAlert(data.message, 'alert-danger')
                                }
                                setBtnClasses(meeting_btn, data.result)
                            }
                        });
                    });
                });

                $(function () {
                    $('#btn-dnd').on('click', function () {
                        $.ajax({
                            url: '/dnd',
                            success: function(data) {
                                if (data.result) {
                                    showAlert(data.message, 'alert-success')
                                } else {
                                    showAlert(data.message, 'alert-danger')
                                }
                                setBtnClasses(dnd_btn, data.result)
                            }
                        });
                    });
                });

                $(function () {
                    $('#btn-headphones').on('click', function () {
                        $.ajax({
                            url: '/headphones',
                            success: function(data) {
                                if (data.result) {
                                    showAlert(data.message, 'alert-success')
                                } else {
                                    showAlert(data.message, 'alert-danger')
                                }
                                setBtnClasses(headphones_btn, data.result)
                            }
                        });
                    });
                });

                $(function () {
                    $('#btn-coffee').on('click', function () {
                        $.ajax({
                            url: '/coffee',
                            success: function(data) {
                                if (data.result) {
                                    showAlert(data.message, 'alert-success')
                                } else {
                                    showAlert(data.message, 'alert-danger')
                                }
                                setBtnClasses(coffee_btn, data.result)
                            }
                        });
                    });
                });

                $(function () {
                    $('#btn-reset').on('click', function () {
                        $.ajax({
                            url: '/reset',
                            success: function(data) {
                                if (data.result) {
                                    showAlert(data.message, 'alert-success')
                                } else {
                                    showAlert(data.message, 'alert-danger')
                                }
                            }
                        });
                    });
                });

                $(function () {
                    $('#btn-mode-switch').on('click', function () {
                      $('body').toggleClass('dark-mode');
                      $('table').toggleClass('table-dark');
                    });
                });
            });
        </script>
    </body>
</html>
)=====";
