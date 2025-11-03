# labs41_gen STM32CubeIDE project

이 폴더는 `code/cnc/labs41 - gen` 펌웨어를 STM32CubeIDE에서 바로 빌드할 수 있도록 준비한 Makefile 기반 프로젝트입니다.

## 요구 사항
- STM32CubeIDE 1.10 이상 (GNU Arm Embedded Toolchain 포함)
- STM32CubeIDE에서 `arm-none-eabi-gcc` 툴체인을 찾을 수 있도록 기본 설정이 되어 있어야 합니다.

## 가져오기 방법
1. STM32CubeIDE를 실행하고 원하는 워크스페이스를 선택합니다.
2. **File &gt; Import...** 메뉴를 열고 **Existing Projects into Workspace**를 선택합니다.
3. **Select root directory**에 `code/cnc/labs41 - gen/STM32CubeIDE` 경로를 지정하면 `labs41_gen` 프로젝트가 나타납니다.
4. 프로젝트를 선택하고 **Finish**를 누르면 CubeIDE 워크스페이스에 추가됩니다.

## 빌드
- 프로젝트를 우클릭하고 **Build Project**를 선택하거나, `Project &gt; Build All`을 실행합니다.
- 빌드가 성공하면 `STM32CubeIDE/build` 폴더 아래에 다음 산출물이 생성됩니다.
  - `labs41_gen.elf`
  - `labs41_gen.hex`
  - `labs41_gen.bin`

## 메모
- 프로젝트는 기존 Keil 설정과 동일하게 `USE_STDPERIPH_DRIVER`, `STM32F2XX`, `SYSCLK_FREQ_120MHz` 매크로를 사용합니다.
- 링커 스크립트(`STM32F205ZGTX_FLASH.ld`)는 STM32F205ZG(1MB Flash / 128KB RAM) 메모리 맵을 기준으로 작성되었습니다. 필요 시 플래시/램 크기를 조정하십시오.
- 추가 소스나 드라이버가 필요하면 `Makefile`의 `C_SOURCES` 또는 include 경로를 수정하면 됩니다.
