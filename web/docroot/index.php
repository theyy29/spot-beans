<!doctype html>
<html lang="en">
<head>
    <title>Something</title>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1"/>
    <link href="/css/bootstrap.min.css" rel="stylesheet"/>
    <link href="/css/style.css" rel="stylesheet">
    <!--[if lt IE 9]>
        <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
        <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
</head>

<body>

    <div id="overlay"></div>

    <div id="playlist-btn">
        <button class="btn btn-default" type="button"><span class="glyphicon glyphicon-th"></span></button>
    </div>

    <div class="wrap">

        <div id="playlist" class="resizable resizable1">
            <div id="playlist-controls">
                <span class="glyphicon glyphicon-repeat btn btn-default playlist-control-repeat active"></span>
                <span class="glyphicon glyphicon-random btn btn-default playlist-control-random"></span>
                <br>
                <span class="glyphicon glyphicon-step-backward btn btn-default playlist-control-backward"></span>
                <span class="glyphicon glyphicon-step-forward btn btn-default playlist-control-forward"></span>
            </div>
            <div id="playlist-list">
                <ul class="nav nav-pills nav-stacked" role="tablist">
                    <li class="active"><a href="#">Playlist1</a></li>
                    <li><a href="#">Playlist2</a></li>
                    <li><a href="#">Playlist3</a></li>
                    <li><a href="#">Playlist4</a></li>
                </ul>
            </div>

        </div>

        <div class="resizable resizable2">

            <div id="song-content" class="">

                <div class="container-fluid">
                    <div class="row">
                        <div id="song-info" class="col-xs-7 col-sm-7 col-md-5 col-lg-6">
                            <div class="row">
                                <div class="col-xs-5 col-sm-3 col-md-4 col-lg-2">
                                    <img class="img-responsive song-img" src="/img/dpa-tmp.jpg" alt="Album Cover"> <!-- Obviously a placeholder. -->
                                </div>
                                <div class="song-info-text">
                                    <p>Song Name</p>
                                    <p>Artist Name</p>
                                    <p>Album Name</p>
                                </div>
                            </div>
                        </div>
                        <div id="song-controls" class="col-xs-5 col-sm-5 col-md-7 col-lg-6">
                            <span id="song-control-backward" class="glyphicon glyphicon-step-backward btn btn-default"></span>
                            <span id="song-control-toggle" class="glyphicon glyphicon-pause btn btn-default" onclick="togglePlayPause()"></span>
                            <span id="song-control-forward" class="glyphicon glyphicon-step-forward btn btn-default"></span>
                            <div class="progress">
                                <div class="progress-bar" role="progressbar" aria-valuenow="60" aria-valuemin="0" aria-valuemax="100" style="width: 60%;">
                                </div>
                            </div>
                        </div>
                    </div>
                </div>

            </div>

            <div id="playlist-content" class="">

                <div id="playlist-song-list">
                    <ul class="nav nav-stacked" role="tablist">
                        <li class="playlist-sort">
                            <span class="song-play"></span>
                            <span id="sort-song-name">Song <i class="glyphicon glyphicon-chevron-down"></i></span>
                            <span id="sort-song-artist">Artist <i class="glyphicon glyphicon-chevron-down"></i></span>
                            <span id="sort-song-album">Album <i class="glyphicon glyphicon-chevron-down"></i></span>
                            <span id="song-time">Time <i class="glyphicon glyphicon-chevron-down"></i></span>
                        </li>
                        <li>
                            <span class="glyphicon glyphicon-play btn btn-primary btn-sm song-play dot"></span>
                            <span class="song-name">CSong2</span>
                            <span class="song-artist">BArtist jnnjjdkas sss</span>
                            <span class="song-album">CAlbum name</span>
                            <span class="song-time">time</span>
                        </li>
                        <li>
                            <span class="glyphicon glyphicon-play btn btn-primary btn-sm song-play"></span>
                            <span class="song-name">BSong asdasdsadadas ad sada</span>
                            <span class="song-artist">Artist name</span>
                            <span class="song-album">Album name</span>
                            <span class="song-time">Utime</span>
                        </li>
                    </ul>
                </div>
            </div>
        </div>
    </div>

    <script src="/js/jquery-2.1.1.min.js"></script>
    <script src="/js/jquery-ui-1.9.2.custom.min.js"></script>
    <script src="/js/jquery.dotdotdot.min.js"></script>
    <script src="/js/js.js"></script>
    <script src="/js/f.js"></script>

</body>
</html>
