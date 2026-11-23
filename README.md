# subtitle-synchronization-CLI




C++ OOP CLI tool that synchronizes `.srt` subtitles using a millisecond offset, similar to VLC subtitle synchronization.



## **1. Usage**

### **Apply one synchronization value**

```
./sub_synchronization movie.srt 2000
```

Output: movie_vlc_p2000ms.srt

Output: movie_vlc_p2000ms.srt

```
./sub_synchronization movie.srt -2000
```

Output: movie_vlc_m2000ms.srt

### **Generate all 8 variations**

```
./sub_synchronization movie.srt --list
```

## **Generated files**

8 synchronization variations

```
movie_vlc_m2000ms.srt
movie_vlc_m1500ms.srt
movie_vlc_m1000ms.srt
movie_vlc_m500ms.srt
movie_vlc_p500ms.srt
movie_vlc_p1000ms.srt
movie_vlc_p1500ms.srt
movie_vlc_p2000ms.srt
```

## **2. Synchronization mapping**

VLC correction → actual SRT timestamp shift (opposite direction)

| VLC correction | File timestamp shift |
| -------------- | -------------------- |
| −2000 ms       | +2000 ms later       |
| −1500 ms       | +1500 ms later       |
| −1000 ms       | +1000 ms later       |
| −500 ms        | +500 ms later        |
| +500 ms        | −500 ms earlier      |
| +1000 ms       | −1000 ms earlier     |
| +1500 ms       | −1500 ms earlier     |
| +2000 ms       | −2000 ms earlier     |

## **6. Example using your subtitle**

Original:

```
1
00:00:20,461 --> 00:00:24,124
Only you

2
00:00:25,800 --> 00:00:30,260
Can make this world
seem right
```

If you run:

```
./sub_synchronization movie.srt 2000
```

The result becomes:

```
1
00:00:18,461 --> 00:00:22,124
Only you

2
00:00:23,800 --> 00:00:28,260
Can make this world
seem right
```

Every subtitle entry is shifted, but the duration between its start and end remains the same. Subtitle text and numbering are preserved.

## **3. One important correction in&#x20;**`main.cpp`

Make sure your `main()` declaration is exactly:

```
int main(int argc, char* argv[])
```

or:

```
int main(int argc, char** argv)
```

Both are valid. Do not use `char* argv` alone.

This project uses **OOP**: `SubtitleSynchronizer` owns the synchronization work, while `main()` handles CLI arguments and decides which operation to execute.



TODO :
**automatic detection of encoding (UTF-8/UTF-16),
progress display, and a&#x20;**`--output`**&#x20;directory argument**