
add_executable(perun2

  main.cpp
  wndres.rc

  arguments.cpp
  attribute.cpp
  brackets.cpp
  cache.cpp
  cmd.cpp
  console.cpp
  post-parse-data.cpp
  side-process.cpp
  exception.cpp
  keyword.cpp
  lexer.cpp
  logger.cpp
  perun2.cpp
  terminator.cpp
  token.cpp
  tokens.cpp

  command/com-aggregate.cpp
  command/com-arg.cpp
  command/com-condition.cpp
  command/com-copyto.cpp
  command/com-core-aggr.cpp
  command/com-core.cpp
  command/com-create.cpp
  command/com-def-action.cpp
  command/com-execute.cpp
  command/com-misc.cpp
  command/com-moveto.cpp
  command/com-parse-kw.cpp
  command/com-parse-unit.cpp
  command/com-parse.cpp
  command/com-renameto.cpp
  command/com-struct.cpp
  command/com-time.cpp
  command/com-var.cpp

  context/ctx-aggr.cpp
  context/ctx-file.cpp
  context/ctx-global.cpp
  context/ctx-index.cpp
  context/ctx-location.cpp
  context/ctx-main.cpp

  datatype/between.cpp
  datatype/cast.cpp
  datatype/comparison.cpp
  datatype/definition.cpp
  datatype/incr-constr.cpp
  datatype/math.cpp
  datatype/number.cpp
  datatype/order-limit-one.cpp
  datatype/order.cpp
  datatype/parse-gen.cpp
  datatype/period.cpp
  datatype/time.cpp

  datatype/function/func-aggr.cpp
  datatype/function/func-attr.cpp
  datatype/function/func-bool.cpp
  datatype/function/func-list.cpp
  datatype/function/func-number.cpp
  datatype/function/func-string.cpp
  datatype/function/func-time.cpp

  datatype/generator/gen-bool-compare.cpp
  datatype/generator/gen-bool-constr.cpp
  datatype/generator/gen-bool.cpp
  datatype/generator/gen-definition.cpp
  datatype/generator/gen-double-asterisk.cpp
  datatype/generator/gen-list.cpp
  datatype/generator/gen-number.cpp
  datatype/generator/gen-os-gen.cpp
  datatype/generator/gen-os.cpp
  datatype/generator/gen-period.cpp
  datatype/generator/gen-string.cpp
  datatype/generator/gen-time.cpp

  datatype/parse/parse-asterisk.cpp
  datatype/parse/parse-bool.cpp
  datatype/parse/parse-definition.cpp
  datatype/parse/parse-function.cpp
  datatype/parse/parse-generic.cpp
  datatype/parse/parse-list.cpp
  datatype/parse/parse-number.cpp
  datatype/parse/parse-numlist.cpp
  datatype/parse/parse-period.cpp
  datatype/parse/parse-string.cpp
  datatype/parse/parse-time.cpp
  datatype/parse/parse-timlist.cpp
  datatype/parse/parse-unit.cpp
  datatype/parse/parse-var.cpp

  datatype/text/chars.cpp
  datatype/text/concat.cpp
  datatype/text/like.cpp
  datatype/text/raw.cpp
  datatype/text/regexp.cpp
  datatype/text/resemblance.cpp
  datatype/text/strings.cpp
  datatype/text/text-parsing.cpp
  datatype/text/wildcard.cpp

  os/os-common.cpp
  os/os-linux.cpp
  os/os-mac.cpp
  os/os-posix.cpp
  os/os-windows.cpp

  programs/linux/linux-programs.cpp
  programs/mac/mac-programs.cpp
  programs/windows/pattern.cpp
  programs/windows/programs-data.cpp
  programs/windows/registry.cpp
  programs/windows/start-menu.cpp
  programs/windows/win-programs.cpp

  python3/com-python3.cpp
  python3/python3-processes.cpp
  python3/shared-memory.cpp

  unicode/convert.cpp
  unicode/regex.cpp
)

target_include_directories(perun2 PRIVATE
  ${PROJECT_SOURCE_DIR}/include
  ${ICU_INCLUDE_DIRS}
  ${FFMPEG_INCLUDE_DIRS}
)

target_link_directories(perun2 PRIVATE
  ${FFMPEG_LIBRARY_DIRS}
)

target_link_libraries(perun2 PRIVATE
  ICU::uc
  ICU::i18n
  ${FFMPEG_LIBRARIES}
)
