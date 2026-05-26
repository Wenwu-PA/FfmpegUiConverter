<body>
<div class="container">

<h1>FFmpeg GUI Converter</h1>

<div class="subtitle">
Консольный конвертер медиафайлов с поддержкой GPU (NVENC, AMF, QSV, AV1)
</div>

<div class="card badges">
<span>C++20</span>
<span>CMake</span>
<span>FFmpeg</span>
</div>

<div class="card">
<h2>Запуск</h2>
<pre>./app.exe</pre>
</div>

<div class="card">
<h2>Использование</h2>

<table>
<tr><th>Параметр</th><th>Пример</th></tr>
<tr><td>Входной файл</td><td>"video.mp4"</td></tr>
<tr><td>Выходной файл</td><td>result.mp4</td></tr>
<tr><td>Видео-кодек</td><td>hevc_nvenc</td></tr>
<tr><td>Аудио</td><td>aac</td></tr>
<tr><td>Пресет</td><td>slow</td></tr>
<tr><td>Битрейт</td><td>2000 (0 = авто)</td></tr>
</table>

<p style="color:#8b949e;">Enter = значение по умолчанию</p>
</div>

<div class="card">
<h2>Примеры</h2>

<h3>CPU</h3>
<pre>input.mp4 → output.mp4 | libx264 | aac | medium | 2000k</pre>

<h3>NVENC</h3>
<pre>video.mp4 → result.mp4 | hevc_nvenc | aac | slow | 3000k</pre>

<h3>AV1</h3>
<pre>video.mkv → compressed.mp4 | libsvtav1 | opus | veryslow | 1000k</pre>
</div>

<div class="card">
<h2>Тесты</h2>
<pre>./test_runner.exe</pre>
</div>

<div class="card">
<h2>ООП</h2>
<ul>
<li>Инкапсуляция — скрытые поля (History)</li>
<li>Наследование — тестовый фреймворк</li>
<li>Полиморфизм — разные кодеки</li>
<li>Абстракция — Menu / Converter / Parser</li>
</ul>
</div>

<div class="card">
<h2>Структура проекта</h2>
<pre>
Heading/   - .h
Body/      - .cpp
tests/     - unit tests
ffmpeg/    - optional
</pre>
</div>

<div class="card">
<h2>Кодеки</h2>

<table>
<tr><th>Тип</th><th>Кодеки</th></tr>
<tr><td>CPU</td><td>libx264, libx265, libsvtav1, libvpx-vp9</td></tr>
<tr><td>NVIDIA</td><td>h264_nvenc, hevc_nvenc, av1_nvenc</td></tr>
<tr><td>AMD</td><td>h264_amf, hevc_amf, av1_amf</td></tr>
<tr><td>Intel</td><td>h264_qsv, hevc_qsv</td></tr>
<tr><td>Audio</td><td>aac, mp3, opus, flac, copy</td></tr>
</table>
</div>

</div>
</body>
</html>
