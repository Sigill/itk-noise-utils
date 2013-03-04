#ifndef __itkAdditiveGaussianNoiseImageFilter
#define __itkAdditiveGaussianNoiseImageFilter

#include <itkUnaryFunctorImageFilter.h>
#include <itkConceptChecking.h>
#include <vnl/vnl_sample.h>

namespace itk
{
namespace Functor
{
/** \class AdditiveGaussianNoise
 * \brief Adds gaussian noise to a pixel.
 * \ingroup ITKImageIntensity
 */
template< class TInput, class TOutput >
class AdditiveGaussianNoise
{
public:
  AdditiveGaussianNoise()
    {
    m_Mean = 0.0;
    m_StandardDeviation = 1.0;
    m_OutputMinimum = itk::NumericTraits< TOutput >::min();
    m_OutputMaximum = itk::NumericTraits< TOutput >::max();
    }

  ~AdditiveGaussianNoise() {}

  double GetMean() const
    { return m_Mean; }

  double GetStandardDeviation() const
    { return m_StandardDeviation; }

  void SetMean(double mean)
    { m_Mean = mean; }

  void SetStandardDeviation(double standardDeviation)
    { m_StandardDeviation = standardDeviation; }

  TOutput GetOutputMinimum() const
    { return m_OutputMinimum; }

  TOutput GetOutputMaximum() const
    { return m_OutputMaximum; }

  void SetOutputMinimum( TOutput min )
    { m_OutputMinimum = min; }

  void SetOutputMaximum( TOutput max )
    { m_OutputMaximum = max; }

  bool operator!=(const AdditiveGaussianNoise &other) const
    {
    return m_Mean != other.m_Mean
      || m_StandardDeviation != other.m_StandardDeviation
      || m_OutputMaximum != other.m_OutputMaximum
      || m_OutputMinimum != other.m_OutputMinimum;
    }

  bool operator==(const AdditiveGaussianNoise & other) const
    {
    return !( *this != other );
    }

  inline TOutput operator()(const TInput & A) const
    {
    double v = A + vnl_sample_normal(m_Mean, m_StandardDeviation);
    return static_cast<TOutput>(v < m_OutputMinimum ? m_OutputMinimum : (v > m_OutputMaximum ? m_OutputMaximum : v));
    }

private:
  TOutput m_OutputMinimum;
  TOutput m_OutputMaximum;
  double m_Mean;
  double m_StandardDeviation;
};
} // End namespace Functor

template< class TInputImage, class TOutputImage >
class ITK_EXPORT AdditiveGaussianNoiseImageFilter:
  public
  UnaryFunctorImageFilter< TInputImage, TOutputImage,
                           Functor::AdditiveGaussianNoise<
                             typename TInputImage::PixelType,
                             typename TOutputImage::PixelType > >
{
public:
  /** Standard class typedefs. */
  typedef AdditiveGaussianNoiseImageFilter Self;
  typedef UnaryFunctorImageFilter<
    TInputImage, TOutputImage,
    Functor::AdditiveGaussianNoise< typename TInputImage::PixelType,
                                    typename TOutputImage::PixelType > >  Superclass;

  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  typedef typename TOutputImage::PixelType OutputPixelType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Macro that provides the GetNameOfClass() method */
  itkTypeMacro(AdditiveGaussianNoiseImageFilter, UnaryFunctorImageFilter);

  OutputPixelType GetOutputMinimum() const
    {
    return this->GetFunctor().GetOutputMinimum();
    }

  OutputPixelType GetOutputMaximum() const
    {
    return this->GetFunctor().GetOutputMaximum();
    }

  double GetMean() const
    { return this->GetFunctor().GetMean(); }

  double GetStandardDeviation() const
    { return this->GetFunctor().GetStandardDeviation(); }

  void SetOutputMinimum(OutputPixelType min)
    {
    if ( min == this->GetFunctor().GetOutputMinimum() )
      {
      return;
      }

    this->GetFunctor().SetOutputMinimum(min);
    this->Modified();
    }

  void SetOutputMaximum(OutputPixelType max)
    {
    if ( max == this->GetFunctor().GetOutputMaximum() )
      {
      return;
      }

    this->GetFunctor().SetOutputMaximum(max);
    this->Modified();
    }

  void SetMean(double mean)
    {
    if ( mean == this->GetFunctor().GetMean() )
      {
      return;
      }

    this->GetFunctor().SetMean(mean);
    this->Modified();
    }

  void SetStandardDeviation(double standardDeviation)
    {
    if ( standardDeviation == this->GetFunctor().GetStandardDeviation() )
      {
      return;
      }

    this->GetFunctor().SetStandardDeviation(standardDeviation);
    this->Modified();
    }

  void PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf(os, indent);
    os << indent << "Mean: " << static_cast<typename NumericTraits<double>::PrintType>(GetMean()) << std::endl;
    os << indent << "StandardDeviation: " << static_cast<typename NumericTraits<double>::PrintType>(GetStandardDeviation()) << std::endl;
    os << indent << "OutputMinimum: " << static_cast<typename NumericTraits<double>::PrintType>(GetOutputMinimum()) << std::endl;
    os << indent << "OutputMaximum: " << static_cast<typename NumericTraits<double>::PrintType>(GetOutputMaximum()) << std::endl;
    }

#ifdef ITK_USE_CONCEPT_CHECKING
  itkConceptMacro( InputConvertibleToDoubleCheck,
    ( Concept::Convertible< typename TInputImage::PixelType, double > ) );
  itkConceptMacro( DoubleConvertibleToOutputCheck,
    ( Concept::Convertible< double, OutputPixelType > ) );
#endif

protected:
  AdditiveGaussianNoiseImageFilter() {}
  virtual ~AdditiveGaussianNoiseImageFilter() {}

private:
  AdditiveGaussianNoiseImageFilter(const Self &); //purposely not implemented
  void operator=(const Self &);     //purposely not implemented
};

} // End namespace itk

#endif /* __itkAdditiveGaussianNoiseImageFilter */
