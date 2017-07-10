#include "Text.h"



Text::Text()
  :m_label(nullptr),
  m_outputText(nullptr)
{
}


Text::~Text()
{
}

bool Text::Initialize(char *label, char *value_default, int position_x, int position_y, const DirectX::XMVECTORF32& color, bool shadow)
{
  // set label string
  char temp_str[16];
  strcpy_s(temp_str, label);
  strcat_s(temp_str, ": ");
  
  size_t label_size = strlen(temp_str) + 1;

  m_label = new char[label_size];
  if (!m_label) return false;

  strcpy_s(m_label, sizeof(char) * label_size, temp_str);

  // set default output string
  m_outputText = new wchar_t[16];
  if (!m_outputText) return false;

  char output_str[16];
  strcat_s(temp_str, value_default);

  size_t newsize = strlen(temp_str) + 1;
  strcpy_s(output_str, sizeof(char)*newsize, temp_str);
  
  size_t converted_chars = 0;
  mbstowcs_s(&converted_chars, m_outputText, newsize, output_str, _TRUNCATE);

  m_posx = position_x;
  m_posy = position_y;
  
  m_color = color;

  return false;
}

wchar_t * Text::GetText() const
{
  return m_outputText;
}

DirectX::XMVECTORF32 Text::GetColor() const
{
  return m_color;
}

void Text::SetColor(const DirectX::XMVECTORF32& color)
{
  m_color = color;
}

void Text::Shutdown()
{
  SafeDelete(m_label);

  if (m_outputText)
  {
    delete[] m_outputText;
    m_outputText = 0;
  }
}

bool Text::UpdateText(int new_val)
{
  char temp_str[16];
  _itoa_s(new_val, temp_str, 10);

  char final_str[16];
  strcpy_s(final_str, m_label);
  strcat_s(final_str, temp_str);

  size_t newsize = strlen(final_str) + 1;
  size_t converted_chars = 0;

  mbstowcs_s(&converted_chars, m_outputText, newsize, final_str, _TRUNCATE);

  return true;
}

int Text::GetPositionX() const
{
  return m_posx;
}

int Text::GetPositionY() const
{
  return m_posy;
}
